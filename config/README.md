Configs description
====================

# ProjectConfig.cmake

| LOG_USE_COLOR | Result |
|---:|---:|
| 0 | No color on RTT log messages |
| 1 | Color in RTT log, SysView is not parsing it | 

| LOG_REDIRECT | Result |
|---:|---:|
| RTT_JLINK | RTT log via JLINK on serial|
| RTT_LUART | RTT log via RT1051 UART on serial| 
| RTT_SYSTEMVIEW | RTT log via JLINK on SystemView | 

| SYSTEM_VIEW_ENABLED | Result |
|---:|---:|
| 0 | SystemView not enabled and code not included|
| 1 | SystemView enabled and code included | 