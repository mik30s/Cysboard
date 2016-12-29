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

###main.html###

<html>
<head>
    <title>Cysboard</title>
    <meta name="position_x" content="0">
    <meta name="position_y" content="0">
    <meta name="width"  content="500">
    <meta name="height" content="740">
    <meta name="time" content="1">
</head>
<body>
    <style>
       html { 
           background: transparent; 
           margin: 0px;
           padding: 0px;
       }

       body  { 
       	 position: relative;
         color: #a7b7b3;
         border-width: 0px;
         border-color: rgba(10, 10, 22, 0.9);
         border-style: solid;
         padding: 0px 5px 0px 2px;
         margin: 0px;
         font-family: "Segoe UI", Tahoma, Helvetica, sans-serif;
         width: 340px;
         height: 100%;
         background: rgba(10, 10, 15, 0.2);
         box-shadow: 0px 0px 20px rgba(10,10, 10, 0.8);
      	}

      	...
            
    </style>
    <div id="header">

    </div>
    <div id="content">
	   <!-- os info -->
	    <div .section #osinfo>
	        <div .section-header>SYSTEM</div>
	        <div .section-content>
	        	<div .font-linux .section-content-child .section-image>
	        	</div>
	        	<div .section-content-child>
	        		<p>name: <span #os_name></span></p>
	        		<p>distro: <span #os_distro_name></span></p>
	        		<p>uptime: <span #os_uptime></span></p>
	        	</div>
	        </div>
	    </div>
	    <!-- cpu info -->
	    <div .section #cpuinfo>
	        <div .section-header>CPU</div>
	        <p>model: <span #cpu_name></span></p>
	        <p>vendor: <span #cpu_vendor></span></p>
	        <p>arch: <span #cpu_arch></span></p>
	        <p>usage: <span #cpu_usage></span> %</p>
	    </div>
	    <!-- mem info -->
	    <div .section #meminfo>
	        <div .section-header>RAM</div>
	        <p>free: <span #mem_free></span> KB</p>
	        <p>used: <span #mem_used></span> KB</p>
	    </div>
    </div>
    <div id="footer">
    	<img src="/home/rubiqz/.config/cysboard/crysis_logo.png" />
    </div>
</body>
</html>



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

