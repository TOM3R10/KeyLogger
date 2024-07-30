#!/bin/bash

# Variables
KERNEL_MODULE_DIR="Kernel_Module"
CLIENT_DIR="Client"
MODULE_NAME="KernelModule"
MODULE_FILE="${KERNEL_MODULE_DIR}/${MODULE_NAME}.ko"
CLIENT_EXECUTABLE="${CLIENT_DIR}/Client.o"
MODULE_INSTALL_PATH="/lib/modules/$(uname -r)/kernel/extra"
CONF_FILE="/etc/modules-load.d/${MODULE_NAME}.conf"

# Check if the module file exists
if [ ! -f "$MODULE_FILE" ]; then
  echo "Module file ${MODULE_FILE} not found!"
  exit 1
fi

# Check if the client executable exists
if [ ! -f "$CLIENT_EXECUTABLE" ]; then
  echo "Client executable ${CLIENT_EXECUTABLE} not found!"
  exit 1
fi

# Create the directory for the module if it does not exist
if [ ! -d "$MODULE_INSTALL_PATH" ]; then
  sudo mkdir -p "$MODULE_INSTALL_PATH"
fi

# Copy the module to the directory
sudo cp "$MODULE_FILE" "$MODULE_INSTALL_PATH"

# Update module dependencies
sudo depmod -a

# Create a configuration file for module loading at boot
echo "$MODULE_NAME" | sudo tee "$CONF_FILE"

# Load the module
sudo modprobe "$MODULE_NAME"

# Run the client executable
"$CLIENT_EXECUTABLE" &

echo "Kernel module ${MODULE_NAME} installed and loaded."
echo "Client executable ${CLIENT_EXECUTABLE} is running."

exit 0
