# SmartNetworking
Wireless Communication Network Project

Overview

This project aims to create a decentralized wireless communication network using ESP32 microcontrollers. The network allows each node to communicate with one another without the need for an internet connection. Each node can relay messages until they reach a designated node that has internet connectivity. This node will then update changes to a centralized database and resolve any queries.

Objectives

Decentralized Communication: Establish a mesh network where nodes can communicate directly with each other.
Message Passing: Implement a unidirectional bus topology for efficient message transmission from one node to the next.
Internet Integration: Enable a designated node to connect to the internet for database updates and query resolution.
Robustness: Ensure that the network can function effectively even in areas with limited or no internet access.
How It Works
Node Communication: Each node is equipped with an ESP32 microcontroller, which utilizes ESP-NOW for wireless communication. This allows for low-latency messaging between devices.

Bus Topology: The network employs a bus topology where messages are passed from one node to the next. Each node knows the MAC address of its neighbor, ensuring messages are forwarded in a specified order.

Internet Connectivity: Once a message reaches the designated internet-enabled node, it will be processed and the necessary updates will be made to the centralized database.

Database Updates: The internet-connected node will send updates, resolve queries, and synchronize data back to the other nodes as needed.

Features
ESP-NOW Communication: Allows for efficient and low-power message transmission between nodes.
Custom Message Structure: Each message contains the initiator's MAC address and the command to be executed, ensuring clear communication.
LED Control: Each node has a built-in LED that can be controlled based on the messages received, providing immediate feedback on command execution.
Serial Monitoring: Each node outputs communication logs to the Serial Monitor for easy debugging and monitoring of the network's status.
