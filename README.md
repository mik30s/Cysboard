Cysboard 
=========
A lightweight system monitor similar to conky but with html and css for themes

<img src="https://travis-ci.org/mike168m/Cysboard.svg?branch=master"/>

## Usage
1. Create a theme file called main.html in ~/.config/cysboard/ (There must be a file name 'main.html').
2. Add your html code with any of the listed ids below in html tags for device info.
3. Run cysboard.

If a 'main.html' file does not exist in the config directory cysboard will start with a default theme.

## Compiling  
You must have cmake >= 3.1 and gcc >= 5.4 in order to compile

1. mkdir build
2. cmake .
3. make


## Examples
<div align=center>
<img src="https://cloud.githubusercontent.com/assets/3809183/22609448/dd8ce142-ea26-11e6-845d-a6d67f8a1bd1.png" />
<img src="https://cloud.githubusercontent.com/assets/3809183/22609058/38ead604-ea25-11e6-9781-ad25d0be30cb.png" />
<img src="https://cloud.githubusercontent.com/assets/1056963/25584994/b8e532f2-2e98-11e7-84ea-95e760a1acb4.png" />
</div>

Use ids like the following 

```html
<html>
<head>
    <title>Cysboard</title>
    <meta name="position_x" content="3400">
    <meta name="position_y" content="50">
    <meta name="width"  content="350">
    <meta name="height" content="350">
    <meta name="time" content="1">
    <link rel=stylesheet href="path/to/style.css" />
</head>
<body>
    <div
       <!-- os info -->
        <div class="section" #osinfo>
            <div >SYSTEM</div>
            <div class="section-content-child">
                <p>name:   <span id="os_name"></span></p>
                <p>distro: <span id="os_distro_name"></span></p>
                <p>uptime: <span id="os_uptime"></span></p>
            </div>
        </div>
        <!-- cpu info -->
        <div id="cpuinfo">
            <div>CPU</div>
            <p>model:  <span id="cpu_name"></span></p>
            <p>vendor: <span id="cpu_vendor"></span></p>
            <p>arch:   <span id="cpu_arch"></span></p>
            <p>usage:  <span id="cpu_usage"></span> %</p>
        </div>
        <!-- mem info -->
        <div id="meminfo">
            <div>RAM</div>
            <p>free: <span id="mem_free"></span> KB</p>
            <p>used: <span id="mem_used"></span> KB</p>
        </div>
        <div>
            <p>Program 1 Output: <span id="exec_1" cmd="path/to/program"></span></p>
            <p>Program 2 Output: <span id="exec_2" cmd="path/to/program"></span></p>	    	
        </div>
        <!-- album art and other stuff -->
        <!-- you can change the img src tag with shell scripts like this -->
        <div>
            <img class="album-pic" id="exec_3" cmd="/home/melchor9000/.config/cysboard/dbus.sh 4">
        </div>
    </div>
</body>
</html>
```
Please see sources/resources/default.html for a working theme. 

## IDs
A list of currently supported information from sources. Add any of
them as an *id* attribute for a tag in your theme file.

ID             | Info
-------------- | -----------------
cpu_name       | The CPU's name                                 
cpu_usage      | The total usage of the CPU in percentages      
cpu_arch       | The CPU's architecture                         
cpu_vendor     | The CPU vendor eg. Intel, AMD                  
cpu_num_cores  | The number of physical cores                
mem_free       | The amount of free memory in KB, MB or GB                
mem_used       | The amount of used memory in KB, MB or GB                
mem_swap_total | The amount of swap memory in KB, MB or GB                
mem_total      | The total amount of physical memory available
os_name        | The name of the OS        
os_distro_name | The distribution name (*Linux only*)       
os_uptime      | The total amount of time since last boot
exec_#         | Execute a program and display its output eg. *exec_0, exec_1* etc.
cpu_usage_#    | Get a cpu core's usage percentage eg. *cpu_usage_0, cpu_usage_1* etc.

Some IDs require attributes and can be used like this

```html
<p>Free RAM: <span id="mem_free" mul="MB"></span></p>
```

Atrribute       | Description                       | Applicable ID(s)
----------------| --------------------------------- | ---------------------------------------------
mul             | Display value in MB, KB or GB     | mem_free, mem_used, mem_swap_total, mem_total
cmd             | Path of program to execute        | exec_#

## Project structure
The source code for the project is located in the source directory.
The header file `cysboard.h` contains the main update function for the program. 
Objects for information from or about the cpu, os, memory and others are initialized in this class. 
All the other files house classes for gathering information from various sources. For example `cpuinfo_linux.cpp` has functions for gathering cpu info on a system. The header file `util.h` holds functions for common and repetitive tasks. 

## Known Issues
1. Program crashes sometimes when editing themes.

## Todo
1. GPU, Disk and Network information.
2. Add graph and bar controls like conky. 
3. Run commands from theme file.
