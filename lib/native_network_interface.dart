import 'dart:ffi';
import 'dart:io';
import 'package:ffi/ffi.dart';
import 'src/native_network_interface_bindings_generated.dart' show NativeNetworkInterfaceBindings, NativeNetworkInterface, NativeIPv4, NativeIPv6;

// --- Your pure Dart classes remain the same ---

class IPv4 {
  final String address;
  final int prefixLength;
  IPv4(this.address, this.prefixLength);
  @override
  String toString() => '$address/$prefixLength';
}

class IPv6 {
  final String address;
  final int prefixLength;
  IPv6(this.address, this.prefixLength);
  @override
  String toString() => '$address/$prefixLength';
}

class NetworkInterface {
  final int index;
  final String name;
  final String description;

  final List<IPv4> ipv4Addresses;
  final List<IPv6> ipv6Addresses;
  final String macAddress;

  final bool isUp;
  final bool isRunning;
  final bool isLoopback;
  final bool isBroadcast;
  final bool isMulticast;
  final int mtu;

  NetworkInterface({
    required this.index,
    required this.name,
    required this.description,
    required this.ipv4Addresses,
    required this.ipv6Addresses,
    required this.macAddress,
    required this.isUp,
    required this.isRunning,
    required this.isLoopback,
    required this.isBroadcast,
    required this.isMulticast,
    required this.mtu,
  });
}

// --- The updated NetworkManager class ---

class NetworkManager {
  /// The bindings to the native functions in your C library.
  late final NativeNetworkInterfaceBindings _bindings;

  NetworkManager() {
    // The `ffiPlugin: true` flag in pubspec.yaml ensures that the native
    // library is bundled with the app and loaded into the process.
    // `DynamicLibrary.process()` gives us a handle to it.
    final dylib = Platform.isAndroid ?
        DynamicLibrary.open('libnetwork_interface.so') :
        DynamicLibrary.process();
    _bindings = NativeNetworkInterfaceBindings(dylib);
  }

  /// Fetches all network interfaces from the native library.
  List<NetworkInterface> listInterfaces() {
    // Allocate pointers to receive the C results.
    // Note: We use the generated `_NetworkInterface` type here.
    final pInterfaces = calloc<Pointer<NativeNetworkInterface>>();
    final pCount = calloc<Size>();

    try {
      // Call the C function via the generated bindings
      final result = _bindings.get_network_interfaces(pInterfaces, pCount);
      if (result != 0) {
        throw Exception('Native function get_network_interfaces failed with exit code $result');
      }

      final interfacesPtr = pInterfaces.value;
      final count = pCount.value;

      if (interfacesPtr == nullptr) {
        return [];
      }

      // Convert the C data to a list of Dart objects
      final interfaces = <NetworkInterface>[];
      for (int i = 0; i < count; i++) {
        // Use the + operator for pointer arithmetic
        final ifaceC = (interfacesPtr + i).ref;

        // Convert IPv4 addresses
        final ipv4s = <IPv4>[];
        for (int j = 0; j < ifaceC.ipv4_count; j++) {
          final ipv4C = (ifaceC.ipv4 + j).ref;
          ipv4s.add(IPv4(
            ipv4C.ipv4_addr.cast<Utf8>().toDartString(),
            ipv4C.prefix_length,
          ));
        }

        // Convert IPv6 addresses
        final ipv6s = <IPv6>[];
        for (int j = 0; j < ifaceC.ipv6_count; j++) {
          final ipv6C = (ifaceC.ipv6 + j).ref;
          ipv6s.add(IPv6(
            ipv6C.ipv6_addr.cast<Utf8>().toDartString(),
            ipv6C.prefix_length,
          ));
        }

        // Convert MAC address
        final mac = ifaceC.mac_addr;
        final macString = List
          .generate(6, (i) => mac[i].toRadixString(16).padLeft(2, '0'))
          .join(':');

        // Add the final, converted Dart object to the list
        interfaces.add(NetworkInterface(
          index: ifaceC.index,
          name: ifaceC.name.cast<Utf8>().toDartString(),
          description: ifaceC.description.cast<Utf8>().toDartString(),
          ipv4Addresses: ipv4s,
          ipv6Addresses: ipv6s,
          macAddress: macString,
          isUp: ifaceC.is_up,
          isRunning: ifaceC.is_running,
          isLoopback: ifaceC.is_loopback,
          isBroadcast: ifaceC.is_broadcast,
          isMulticast: ifaceC.is_multicast,
          mtu: ifaceC.mtu,
        ));
      }

      // Free the memory allocated by the C library.
      _bindings.free_network_interfaces(interfacesPtr, count);

      return interfaces;
    } finally {
      // Free the pointers we allocated on the Dart side.
      calloc.free(pInterfaces);
      calloc.free(pCount);
    }
  }
}
