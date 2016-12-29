Cysboard 
=========
A lightweight system monitor similar to conky but with html and
css for themes

#Usage#
1. Create a theme file called main.html in ~/.config/cysboard/ (There must be a file name 'main.html')
2. Add your html code with any of the listed ids below in html tags for device info.
3. Run cysboard.

##Examples##
![cysboard_default](https://cloud.githubusercontent.com/assets/3809183/21536018/bdfcee36-cd42-11e6-95de-2e8491c49ce5.png)

##IDs##
A list of currently supported information from sources. Add any of
them as an *id* attribute for a tag in your theme file.

cpu_name -  The CPU's name                                 
cpu_usage - The total usage of the CPU in percentages      
cpu_arch  - The CPU's architecture                         
cpu_vendor - The CPU vendor eg. Intel, AMD                  
cpu_num_cores - The number of physical cores                
mem_free - The amount of free memory in KB                
mem_used - The amount of used memory in KB                
mem_swap_total - The amount of swap memory in KB                
mem_total - The total amount of physical memory available  
os_name - The name of the OS eg. linux or windows        
os_distro_name - The distribution name (*Linux only*)       
os_uptime - The total amount of time since last boot     



####If a 'main.html' file does not exist in the config directory cysboard will start with a default theme.####

