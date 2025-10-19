# IDDI
this is my repo to automatically hash any PE file that is Created in /root || /home on a linux sustem

## Description

An in-depth paragraph about your project and overview of use.

## Getting Started

## Freatures
- Recursive monitoring of /home and /root
- PE file detection (MZ header)
- MD5 hashing
- Systemd service integration
- Log rotation support

### Dependencies

```bash
sudo apt update
sudo apt install libssl-dev

```

### Installing/Compiling

```bash
// compile and move system components
gcc pe_monitor.c -o pe_monitor -lssl -lcrypto
sudo mv pe_monitor /usr/local/bin/

// start the pe monitoring service
sudo systemctl daemon-reload
sudo systemctl enable pe-monitor.service
sudo systemctl start pe-monitor.service

// check the status with systemctl 
sudo systemctl status pe-monitor.service
```


### Executing program

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

## Help

File directory break down 
```text
pe-monitor/
├── src/
│   └── pe_monitor.c          # Main C source code
├── scripts/
│   ├── install.sh            # Main installation script
│   ├── uninstall.sh          # Uninstallation script
│   └── build.sh              # Build script for development
├── systemd/
│   └── pe-monitor.service    # Systemd service unit file
├── packaging/
│   ├── debian/               # Debian package structure (for .deb)
│   │   ├── control
│   │   ├── rules
│   │   └── postinst
│   └── rpm/                  # RPM package structure (optional)
├── docs/
│   ├── README.md            # Project documentation
│   ├── INSTALL.md           # Installation instructions
│   └── USAGE.md             # Usage and troubleshooting
├── tests/
│   ├── test_pe_files/       # Sample PE files for testing
│   │   ├── test1.exe
│   │   └── test2.dll
│   ├── test_non_pe/         # Non-PE files for negative testing
│   │   └── test.txt
│   └── test_monitor.py      # Python script to test monitoring
├── config/
│   ├── pe-monitor.conf      # Configuration file (optional)
│   └── logrotate.d/
│       └── pe-monitor       # Log rotation config
├── man/
│   └── man1/
│       └── pe_monitor.1     # Man page for the binary
└── LICENSE                  # License file (MIT, GPL, etc.)
```

## Authors

Contributors names and contact info

ex. Dominique Pizzie  
ex. [@DomPizzie](https://twitter.com/dompizzie)

## Version History

* 0.2
    * Various bug fixes and optimizations
    * See [commit change]() or See [release history]()
* 0.1
    * Initial Release

## Notes
- This monitors for creations only. If you need modifications or other events, adjust the IN_CREATE mask.
- inotify has limits on the number of watches (check /proc/sys/fs/inotify/max_user_watches and increase if needed with sysctl).
- The program runs indefinitely as a daemon via systemd.
- Error handling is basic; it skips inaccessible directories (e.g., permission denied).
- For testing, create a .exe file in /home and check the log.

## Build and Distribution
For Development:
```bash
bashchmod +x scripts/build.sh scripts/install.sh
./scripts/build.sh  # Compiles pe_monitor
sudo ./scripts/install.sh  # Full installation
```
For Distribution:
```bash
Debian Package:
bash# From project root
dpkg-buildpackage -us -uc
sudo dpkg -i ../pe-monitor_1.0.0_amd64.deb
```
Binary Distribution:
Create a tarball:
```bash
bashtar -czf pe-monitor-1.0.0.tar.gz --exclude="*.git*" .
```

Version Control Structure:
text.git/
├── hooks/
│   └── pre-commit          # Linting and testing hook
├── ignore
└── modules/
Installation Workflow:

- User downloads pe-monitor-1.0.0.tar.gz
- Extracts and runs sudo ./scripts/install.sh
- Service starts automatically
- Logs appear in /var/log/pe_monitor.log

This structure makes the project:

Professional: Clear separation of concerns
Maintainable: Easy to find and modify components
Distributable: Supports packaging systems
Testable: Includes testing framework
Documented: Comprehensive docs and man pages
User-friendly: Simple installation process

The modular design also allows easy extension (adding config files, different hash algorithms, more event types, etc.).

## License

This project is licensed under the [The Unlicense] License - see the LICENSE.md file for details

## Acknowledgments
Inspiration, code snippets, etc.
