import subprocess


def pair(config):
    cmdLine = [config.sixpair]
    code = subprocess.call(cmdLine)
    if code == 0:
        print("sixpair succeded")
    else:
        print("sixpair filed: exit code {0}".format(code))


def detach(mac):
    if not mac:
        print("missing MAC address")
        return

    cmdLine = ["sudo", "hcitool", "dc", mac]
    code = subprocess.call(cmdLine)
    if code == 0:
        print("successfully detached")
    else:
        print("detach filed: exit code {0}".format(code))
