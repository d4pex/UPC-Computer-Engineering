# Computer Networks & Protocols 🌐

## 📡 Module Overview
Comprehensive analysis of the **TCP/IP Stack** and **OSI Model**. This module focuses on packet-level inspection, network architecture design (LAN/WAN), and wireless communication standards (802.11).

**Red Team Relevance:**
Deep understanding of networking is mandatory for **Network Pivoting**, **Man-in-the-Middle (MitM)** attacks, **Traffic Sniffing**, and bypassing firewalls via packet fragmentation.

## 📂 Labs & Traffic Analysis

### 1. 🦈 Traffic Analysis & TCP/IP
* **File**: `Network-Sniffing-and-Routing.pdf`
* **Tools**: **Wireshark**, `tcpdump`.
* **Skills**:
    * **Packet Inspection**: Analyzing the TCP 3-Way Handshake (`SYN`, `SYN-ACK`, `ACK`) to diagnose connection issues.
    * **Filters**: Using capture filters to isolate specific traffic (Broadcast, Multicast, Unicast).
    * **Port Mirroring**: Understanding how traffic is replicated in switched networks.

### 2. 🔐 Secure Transport & Fragmentation
* **File**: `TCP-IP-Stack-Analysis-SSH.pdf`
* **Skills**:
    * **SSH & SCP**: Secure data transfer and key management (essential for **Lateral Movement**).
    * **IP Fragmentation**: Analyzing how large packets are split (MTU analysis), a concept used in **IDS Evasion** attacks.
    * **Encapsulation**: Efficiency calculation of protocol headers.

### 3. 📶 Wireless Networks (WLAN) & Infrastructure
* **File**: `Wireless-LAN-Architecture-Mikrotik.pdf`
* **Tools**: **Mikrotik RouterOS**, `iperf`.
* **Skills**:
    * **802.11 Analysis**: Understanding Management Frames (Beacons, Probes) used in **Wardriving**.
    * **Infrastructure Mode**: Configuring Bridges, NAT, and Firewalls on Mikrotik devices.
    * **Performance**: Bandwidth stress testing using `iperf`.

---
*Tech Stack: Wireshark, Linux Networking Stack, Mikrotik, SSH.*