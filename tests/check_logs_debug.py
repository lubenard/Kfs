import os

filename = "log.txt"

if not os.path.isfile(filename):
    print('File does not exist.')

f = open(filename, "r")

lines = f.read()

logsToSearch = ["GDT has been initialised at 0x800", "PIT has been initialised",
                "RTC initialised", "Keyboard init"]

for item in logsToSearch:
    if item in lines:
        print("'{}' found in logs\n".format(item))
    else:
        print("'{}' NOT found in logs\n".format(item))
        exit(1)
exit(0)
