import 'package:flutter/material.dart';
import 'dart:async';

import 'package:native_network_interface/native_network_interface.dart' as native_network_interface;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  late native_network_interface.NetworkManager manager;
  late List<native_network_interface.NetworkInterface> interfaces;
  String interfacesString = "";

  @override
  void initState() {
    super.initState();
    manager = native_network_interface.NetworkManager();
    interfaces = manager.listInterfaces();
    for (final iface in interfaces) {
      interfacesString += '--- ${iface.name} ---\n';
      interfacesString += '  Index: ${iface.index}\n';
      interfacesString += '  MAC: ${iface.macAddress}\n';
      interfacesString += '  MTU: ${iface.mtu}\n';
      interfacesString += '  Is Up: ${iface.isUp}\n';
      interfacesString += '  IPv4: ${iface.ipv4Addresses}\n';
      interfacesString += '  IPv6: ${iface.ipv6Addresses}\n';
      interfacesString += '\n';
    }
  }

  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(fontSize: 16);
    const spacerSmall = SizedBox(height: 10);
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Network Interfaces Example'),
        ),
        body: SingleChildScrollView(
          child: Container(
            padding: const EdgeInsets.all(10),
            child: Column(
              children: [
                spacerSmall,
                Text(
                  'Interfaces:\n$interfacesString',
                  style: textStyle,
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
