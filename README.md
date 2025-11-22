# message-platform-in-unix

# Terminal-Based Message Distribution System

A terminal-based messaging platform built in **C** for **UNIX/Linux**, designed to demonstrate **interprocess communication**, **named pipes (FIFOs)**, and **process synchronization**.  
The system simulates a small-scale messaging service where multiple users can exchange short messages organized by topics in real time.

---

## 📋 Overview

The project uses a **client–server architecture** with two main programs:

- **Manager** — Handles topics, users, and message distribution.  
- **Feed** — Client interface allowing users to send and receive messages from various topics.  

Features include:

- Dynamic topic subscription and publication  
- Real-time message delivery to all subscribers  
- Persistent messages stored for a defined lifetime and reloaded when the server restarts  

---

## 🛠️ Technologies Used

- **C (GCC)** — Core implementation language  
- **UNIX System Calls** — File and process management  
- **Named Pipes (FIFOs)** — Interprocess communication  
- **Signals** — Event handling and safe termination  
- **Makefile** — Build and cleanup automation  
- **Linux Terminal UI** — Console-based user interaction  

---

## 🔬 Main Features

- **Topic-Based Messaging** — Publish or subscribe to specific topics  
- **Real-Time Delivery** — Messages instantly distributed to subscribers  
- **Persistent Messages** — Messages can survive restarts for a limited lifetime  
- **Administrative Control** — Monitor, lock/unlock topics, and remove users from the manager console  
- **Multi-Client Simulation** — Multiple feeds (users) can run concurrently in separate terminals  
- **Safe Termination** — Manager and feed processes handle shutdown gracefully  

---

## 💻 Commands

### In the **Feed** client
| Command | Description |
|---------|-------------|
| `topics` | List all available topics |
| `msg <topic> <duration> <message>` | Send a message to a topic (`duration = 0` for non-persistent) |
| `subscribe <topic>` | Subscribe to a topic |
| `unsubscribe <topic>` | Unsubscribe from a topic |
| `exit` | Leave the platform |

### In the **Manager** console
| Command | Description |
|---------|-------------|
| `users` | List all active users |
| `remove <username>` | Disconnect a user |
| `topics` | Show all topics and message counts |
| `show <topic>` | Display messages in a topic |
| `lock <topic>` | Block new messages in a topic |
| `unlock <topic>` | Unblock a topic |
| `close` | Shut down the system |

---

## 🌐 Environment Variables

The system uses an environment variable to store persistent messages:

```sh
export MSG_FICH=persistentMessagesFile.txt
