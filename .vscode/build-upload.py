import tempfile
import shutil
from distutils import dir_util
from os.path import join
from os import path
import os
import sys
import subprocess
import json

board_vid = '0x1a86'
board_pid = '0x7523'
board_fqdn = 'realtek:AmebaPro2:Ameba_AMB82-MINI'

project_dir = sys.argv[1]  # platformio project directory
working_dir = tempfile.mkdtemp()  # temporary directory
print(working_dir)

project_name = path.basename(path.normpath(project_dir))  # platformio project directory name
sketch_dir = join(working_dir, project_name)

dir_util.copy_tree(join(project_dir, 'src'), sketch_dir)  # move code to the sketch directory

shutil.move(join(sketch_dir, 'main.cpp'), join(sketch_dir, f'{project_name}.ino'))  # move the code entry point to the sketch file

arduino_cli = join(path.dirname(path.realpath(__file__)), 'arduino-cli')

# find amb82 port
ports = json.loads(subprocess.check_output([arduino_cli, 'board', 'list', '--json']))['detected_ports']
port = None
for detected_port in ports:
    port_props = detected_port['port']['properties']
    if 'pid' in port_props and \
        port_props['pid'] == board_pid and \
        port_props['vid'] == board_vid:
        port = detected_port['port']['address']
        print(port)
        break

build_cmd = [
    arduino_cli,
    'compile', sketch_dir,
    '-b', board_fqdn,
    '--libraries', join(project_dir, 'lib')
]

if port is not None:
    build_cmd.append('--upload')
    build_cmd.append('--port')
    build_cmd.append(port)
else:
    print('AMB82 not detected, upload disabled')

for dir in os.listdir(join(project_dir, '.pio', 'libdeps')):  # add every library of every env
    build_cmd.append('--libraries')
    build_cmd.append(join(project_dir, '.pio', 'libdeps', dir))

process = subprocess.Popen(build_cmd, cwd=sketch_dir)
print(' '.join(process.args))
process.wait()

shutil.rmtree(working_dir)