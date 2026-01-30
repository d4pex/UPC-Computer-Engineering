# Internet Technologies & Infrastructure 🌐

## 📡 Module Overview
Hands-on engineering of IP networks, focusing on the configuration of **Routing**, **Switching**, and **Edge Security**. The module uses **Mikrotik RouterOS** devices to implement complex topologies, utilizing concepts like VLSM, Stateful Firewalls, and Dynamic Routing.

**Red Team Relevance:**
* **Network Reconnaissance:** Deep understanding of **Subnetting (VLSM)** and **DNS/DHCP** mechanisms is crucial for mapping internal networks during a pentest.
* **Firewall Evasion:** Knowledge of **Stateful Filtering** (Connection Tracking) and Input/Forward chains helps in designing evasion techniques and C2 channels.
* **Traffic Analysis:** differentiating between normal flow (Throughput/RTT) and anomalies.

## 📂 Projects & Labs

### 1. 🛡️ Firewalling & Security
* **File**: `Lab-Firewall-Stateful-Configuration.pdf`
* **Tech**: Mikrotik Firewall Filter Rules.
* **Skills**:
    * Configuring **Input** vs **Forward** chains.
    * Implementing **Stateful Inspection** (Accept Established/Related, Drop Invalid).
    * analyzing ICMP filtering policies.

### 2. 🗺️ Network Design & Routing
* **File**: `Project-VLSM-Subnetting-Design.pdf` & `Lab-Routing-High-Availability.pdf`
* **Skills**:
    * **VLSM Design**: Calculating optimal subnet masks for hierarchical networks (Development, DMZ, Admin zones).
    * **Routing Tables**: Configuring Static Routes and Gateway Failover detection.
    * **Infrastructure Services**: Deployment of DHCP Servers and DNS resolution analysis (`dig`, ARP tables).

### 3. 📊 Transport & App Layer Analysis
* **File**: `Lab-Port-Scanning-TCP-UDP.pdf`
* **Skills**:
    * **Port Scanning**: Identifying open services using TCP Connect vs UDP scanning techniques (conceptually similar to Nmap).
    * **Flow Analysis**: Measuring Throughput, Jitter, and RTT using `iperf` and Wireshark.
    * **Protocol Inspection**: Analyzing HTTP vs HTTPS (TLS) handshakes.

### 4. 📶 Wireless Infrastructure
* **File**: `Lab-Wireless-Infrastructure-WLAN.pdf`
* **Skills**:
    * SSID configuration and frequency analysis.
    * Understanding Beacon frames and wireless association processes.

---
*Tech Stack: Mikrotik RouterOS, Wireshark, Linux Networking, Iperf, Dig.*