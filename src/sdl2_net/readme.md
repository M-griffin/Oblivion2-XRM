## SDL2_net Inclusion into the Project
We include this project directly for manual patching of the **SO_KEEPALIVE** feature.
- **SO_KEEPALIVE** is a connection maintenance feature. It sends background "probes" during long periods of inactivity to verify that the remote peer is still reachable and hasn't crashed or disconnected without closing the socket.
- Reported as an issue originally by Hawk Hubbard during some testing. 

**SO_KEEPALIVE** is not enabled by default in SDL2_net, nor does SDL2_net provide a built-in cross-platform function to toggle it.
Like most networking libraries, SDL2_net creates a standard TCP socket and leaves it in the default operating system state.

### Why you must enable it manually:
**Disabled by Default:** 

On Windows, Linux, and macOS, the SO_KEEPALIVE socket option is set to FALSE by default.

**Minimalist Design:** 

SDL2_net is designed to be a thin wrapper. It handles the basics of connection and data transfer but does not automatically modify low-level socket options like keep-alives or buffer sizes.

**Default Timeouts are Long:** 

Even if it were enabled by the OS, the default "idle time" before sending a probe is typically 2 hours on Windows and Linux. To make keep-alive useful for a real-time application (like a game or chat app), you need to manually set SO_KEEPALIVE and then configure shorter timeouts (e.g., 30–60 seconds). 

