<!--
// Copyright (c) toby, toby.chs@gmail.com.
function FindAndSelect(selectBox, value)
{
	var i = 0;
	for(i = 0; i < selectBox.length; i++)
	{
		if(selectBox.options[i].value == value)
		{
			selectBox.selectedIndex = i;
			return;
		}
	}
}
function FindNearestAndSelect(selectBox, value)
{
	var i = 0;
	var min;
	var max;

	for(i = 0; i < selectBox.length; i++)
	{
		min = (value * 90) / 100;
		max = (value * 110) / 100;

		if((selectBox.options[i].value > min) && (selectBox.options[i].value < max))
		{
			selectBox.selectedIndex = i;
			return;
		}
	}
}
function SetFormDefaults()
{
	//LOGIN
	if(vloginstat == 0)
	{
		document.getElementById("ploginstat").innerHTML = "Not Logged In";
	}
	else
	{
		document.getElementById("ploginstat").innerHTML = "Logged In";
	}
	
	// WLAN Configure
	document.wlan.ssid.value = vssid;
	FindAndSelect(document.wlan.encrytype, vencrytype);
	// <div>no passwd</div>
	FindAndSelect(document.wlan.staticip, vstaticip);
	document.wlan.wlansip.value = vwlansip;
	document.wlan.wlanmip.value = vwlanmip;
	document.wlan.wlangip.value = vwlangip;
	document.wlan.wlandip.value = vwlandip;
		
	document.getElementById("pssid").innerHTML = vssid;
	switch(vencrytype)
	{
		case 0: document.getElementById("pencrytype").innerHTML = "None"; break;
		case 1: document.getElementById("pencrytype").innerHTML = "WEP-open"; break;
		case 2: document.getElementById("pencrytype").innerHTML = "WEP-share"; break;
		case 3: document.getElementById("pencrytype").innerHTML = "WPA or WPA2"; break;
		default: document.getElementById("pencrytype").innerHTML = "N/A"; break;
	}
	document.getElementById("pwifipassword").innerHTML = "********";
	if(vstaticip == 1) // static ip
	{
		document.getElementById("pstaticip").innerHTML = "Static IP";
	}
	else
	{
		document.getElementById("pstaticip").innerHTML = "DHCP";
	}
	document.getElementById("pwlansip").innerHTML = vwlansip;
	document.getElementById("pwlanmip").innerHTML = vwlanmip;
	document.getElementById("pwlangip").innerHTML = vwlangip;
	document.getElementById("pwlandip").innerHTML = vwlandip;
	
	// WIFI Port Configure
	document.wifi.socketlp.value = vsocketlp;
	document.wifi.socketrp.value = vsocketrp;
	FindAndSelect(document.wifi.socketmode, vsocketmode);
	FindAndSelect(document.wifi.socketprol, vsocketprol);
	document.wifi.socketrip.value = vsocketrip;
	
	document.getElementById("psocketlp").innerHTML = vsocketlp;
	document.getElementById("psocketrp").innerHTML = vsocketrp;
	if(vsocketprol == 1) // udp
	{
		document.getElementById("psocketprol").innerHTML = "UDP";
		document.getElementById("psocketmode").innerHTML = "N/A";
		document.getElementById("psocketrip").innerHTML = vsocketrip;
	}
	else // tcp
	{
		document.getElementById("psocketprol").innerHTML = "TCP";
		if(vsocketmode == 1) // client
		{
			document.getElementById("psocketmode").innerHTML = "Client";
			document.getElementById("psocketrip").innerHTML = vsocketrip;
		}
		else
		{
			document.getElementById("psocketmode").innerHTML = "Server";
			document.getElementById("psocketrip").innerHTML = "N/A";
		}
	}
	
	// Serial Port Configure
	FindNearestAndSelect(document.serial.baudrate, vbaudrate);
	FindAndSelect(document.serial.datasize, vdatasize);
	FindAndSelect(document.serial.parity, vparity);
	FindAndSelect(document.serial.stop, vstop);
	FindAndSelect(document.serial.flow, vflow);
	
	document.getElementById("pbaudrate").innerHTML = vbaudrate;
	document.getElementById("pdatasize").innerHTML = vdatasize;
	switch(vparity)
	{
		case 0: document.getElementById("pparity").innerHTML = "None"; break;
		case 1: document.getElementById("pparity").innerHTML = "Odd"; break;
		case 2: document.getElementById("pparity").innerHTML = "Even"; break;
		case 3: document.getElementById("pparity").innerHTML = "Mark"; break;
		case 4: document.getElementById("pparity").innerHTML = "Space"; break;
		default: document.getElementById("pparity").innerHTML = "N/A"; break;
	}
	document.getElementById("pstop").innerHTML = vstop;
	if(vflow == 0)
	{
		document.getElementById("pflow").innerHTML = "None";
	}
	else
	{
		document.getElementById("pflow").innerHTML = "Hardware";
	}

	SetWLANState();
	SetWIFIState();
}

function SetWLANState()
{
	var encrytype = document.wlan.encrytype.value;
	var staticip = document.wlan.staticip.value;
	var disable;

	if(encrytype == 0)
	{
		disable = true;
	}
	else
	{
		disable = false;
	}
	document.wlan.wifipassword.disabled = disable;

	if(staticip == 0)
	{
		disable = true;
	}
	else
	{
		disable = false;
	}
	document.wlan.wlansip.disabled = disable;
	document.wlan.wlanmip.disabled = disable;
	document.wlan.wlangip.disabled = disable;
	document.wlan.wlandip.disabled = disable;
}
function SetWIFIState()
{
	var f =  get_obj("wifi");
	
	f.socketlp.disabled = false;
	f.socketrp.disabled = false;
	f.socketmode.disabled = false;
	f.socketprol.disabled = false;
	f.socketrip.disabled = false;
	
	
	if(f.socketprol.value == 1) // udp
	{
		f.socketlp.disabled = false;
		f.socketrp.disabled = false;
		f.socketmode.disabled = true;
		f.socketrip.disabled = false;
 	}
	else // tcp
	{
		if(f.socketmode.value == 1) // client
		{
			f.socketlp.disabled = false;
			f.socketrp.disabled = false;
			f.socketrip.disabled = false;
		}
		else // server
		{
			f.socketlp.disabled = false;
			f.socketrp.disabled = false;
			f.socketrip.disabled = true;
		}
	}
}
// marked by toby@2011.09.12
// if the characters of "char_code" is in following ones: 0~9, A~Z, a~z, some control key and TAB.
function __is_comm_chars(char_code)
{
    if (char_code == 0)  return true;                       /* some control key. */
    if (char_code == 8)  return true;                       /* TAB */
    if (char_code >= 48 && char_code <= 57)  return true;   /* 0~9 */
    if (char_code >= 65 && char_code <= 90)  return true;   /* A~Z */
    if (char_code >= 97 && char_code <= 122) return true;   /* a~z */

    return false;
}
function __is_char_in_string(target, pattern)
{
    var len = pattern.length;
    var i;
    for (i=0; i<len; i++)
    {
        if (target == pattern.charCodeAt(i)) return true;
    }
    return false;
}
//if the evt is in the allowed characters.
function __is_evt_in_allow_chars(evt, allow_comm_chars, allow_chars)
{
    var char_code;
    var i;

    if (navigator.appName == 'Netscape'){char_code=evt.which;   }
    else                                {char_code=evt.keyCode; }

    if (allow_comm_chars == "1" && __is_comm_chars(char_code)==true) return true;
    if (allow_chars.length > 0 && __is_char_in_string(char_code, allow_chars)==true) return true;

    return false;
}
//if the characters of "str" are all in the allowed "allow_chars".
function __is_str_in_allow_chars(str, allow_comm_chars, allow_chars)
{
    var char_code;
    var i;

    for (i=0; i<str.length; i++)
    {
        char_code=str.charCodeAt(i);
        if (allow_comm_chars == "1" && __is_comm_chars(char_code) == true) continue;
        if (allow_chars.length > 0 && __is_char_in_string(char_code, allow_chars) == true) continue;
        return false;
    }
    return true;
}

// Get Object by ID.
function get_obj(name)
{
    if (document.getElementById)    return document.getElementById(name);//.style;
    if (document.all)               return document.all[name].style;
    if (document.layers)            return document.layers[name];
}
// generate the radmon str by date.
function generate_random_str()
{
    var d = new Date();
    var str=d.getFullYear()+"."+(d.getMonth()+1)+"."+d.getDate()+"."+d.getHours()+"."+d.getMinutes()+"."+d.getSeconds();
    return str;
}
// this function is used to check if the inputted string is blank or not.
function is_blank(s)
{
    var i=0;
    for(i=0;i<s.length;i++)
    {
        c=s.charAt(i);
        if((c!=' ')&&(c!='\n')&&(c!='\t'))return false;
    }
    return true;
}

// this function is used to check if the string is blank or zero.
function is_blank_or_zero(s)
{
    if (is_blank(s)==true) return true;
    if (is_digit(s))
    {
        var i = parseInt(s, 10);
        if (i==0) return true;
    }
    return false;
}

// this function is used to check if the "str" is a decimal number or not.
function is_digit(str)
{
    if (str.length==0) return false;
    for (var i=0;i < str.length;i++)
    {
        if (str.charAt(i) < '0' || str.charAt(i) > '9') return false;
    }
    return true;
}

// this function is used to check if the value "str" is a hexcimal number or not.
function is_hexdigit(str)
{
    if (str.length==0) return false;
    for (var i=0;i < str.length;i++)
    {
        if (str.charAt(i) <= '9' && str.charAt(i) >= '0') continue;
        if (str.charAt(i) <= 'F' && str.charAt(i) >= 'A') continue;
        if (str.charAt(i) <= 'f' && str.charAt(i) >= 'a') continue;
        return false;
    }
    return true;
}

// convert dec integer string
function decstr2int(str)
{
    var i = -1;
    if (is_digit(str)==true) i = parseInt(str, [10]);
    return i;
}

// convert hex integer string
function hexstr2int(str)
{
    var i = 0;
    if (is_hexdigit(str)==true) i = parseInt(str, [16]);
    return i;
}

// if min <= value <= max, than return true,
// otherwise return false.
function is_in_range(str_val, min, max)
{
    var d = decstr2int(str_val);
    if ( d > max || d < min ) return false;
    return true;
}

// this function convert second to day/hour/min/sec
function second_to_daytime(str_second)
{
    var result = new Array();
    var t;

    result[0] = result[1] = result[2] = result[3] = 0;

    if (is_digit(str_second)==true)
    {
        t = parseInt(str_second, [10]);
        result[0] = parseInt(t/(60*60*24), [10]);   // day
        result[1] = parseInt(t/(60*60), [10]) % 24; // hr
        result[2] = parseInt(t/60, [10]) % 60;      // min
        result[3] = t % 60;                         // sec
    }

    return result;
}

// construct xgi string for doSubmit()
function exe_str(str_shellPath)
{
    var str="";
    myShell = str_shellPath.split(";");
    for(i=0; i<myShell.length; i++)
    {
        str+="&"+"exeshell="+myShell[i];
    }
    return str;
}

// return true is brower is IE.
function is_IE()
{
    if (navigator.userAgent.indexOf("MSIE")>-1) return true;
    return false
}

// make docuement.write shorter
function echo(str)
{
    document.write(str);
}

// same as echo() but replace special characters
function echosc(str)
{
    str=str.replace(/&/g,"&amp;");
    str=str.replace(/</g,"&lt;");
    str=str.replace(/>/g,"&gt;");
    str=str.replace(/"/g,"&quot;");
    str=str.replace(/'/g,"\'");
    str=str.replace(/ /g,"&nbsp;");
    document.write(str);
}

// return false if keybaord event is not decimal number.
function dec_num_only(evt)
{
    if (navigator.appName == 'Netscape')
    {
        if (evt.which == 8) return true;    /* TAB */
        if (evt.which == 0) return true;
        if (evt.which >= 48 && evt.which <= 57) return true;
    }
    else
    {
        if (evt.keyCode == 8) return true;
        if (evt.keyCode == 0) return true;
        if (evt.keyCode >= 48 && evt.keyCode <= 57) return true;
    }
    return false;
}

// return false if keyboard event is not hex number.
function hex_num_only(evt)
{
    if (navigator.appName == 'Netscape')
    {
        if (evt.which == 8) return true;    /* TAB */
        if (evt.which == 0) return true;
        if (evt.which >= 48 && evt.which <= 57) return true;
        if (evt.which > 64 && evt.which < 71) return true;
        if (evt.which > 96 && evt.which < 103) return true;
    }
    else
    {
        if (evt.keyCode == 8) return true;  /* TAB */
        if (evt.keyCode == 0) return true;
        if (evt.keyCode >= 48 && evt.keyCode <= 57) return true;
        if (evt.keyCode > 64 && evt.keyCode < 71) return true;
        if (evt.keyCode > 96 && evt.keyCode < 103) return true;
    }
    return false;
}

// return false if keyboard event is not readable character
function readable_char_only(evt)
{
    if (navigator.appName == 'Netscape')
    {
    if (evt.which == 8) return true;    /* TAB */
    if (evt.which == 0) return true;
    if (evt.which < 33 || evt.which > 126) return false;
    }
    else
    {
    if (evt.keyCode == 8) return true;  /* TAB */
    if (evt.keyCode == 0) return true;
    if (evt.keyCode < 33 || evt.keyCode > 126) return false;
    }
    return true;
}


// make the obj selected, if the value of obj is empty, 'def' will be set as value.
function field_select(obj, def)
{
    if (obj.value == '') obj.value = def;
    obj.select();
}

// make the object be focused, and set the value to 'val'.
function field_focus(obj, val)
{
    if (val != '**') obj.value = val;
    obj.focus();
    obj.select();
}

// make all fields of the obj disabled/enabled. "dis" will be true or false.
function fields_disabled(obj, dis)
{
    var i=0;
    for(i=0; i<obj.length; i++)
    {
        eval("obj["+i+"].disabled="+dis);
    }
}

// for safari select loop
function select_index(obj, val)
{
    var i=0;
    for(i=0; i<obj.length;i++)
    {
        if(eval("obj["+i+"].value")==val)
        {
            obj.selectedIndex=i;
            break;
        }
    }
}

// check if any unicode characters in the string.
function strchk_unicode(str)
{
    var strlen=str.length;
    if(strlen>0)
    {
        var c = '';
        for(var i=0;i<strlen;i++)
        {
            c = escape(str.charAt(i));
            if(c.charAt(0) == '%' && c.charAt(1)=='u')
                return true;
        }
    }
    return false;
}

function strchk_url(str)
{
    if (__is_str_in_allow_chars(str, 1, "/.:_-?&=")) return true;
    return false;
}

function strchk_hostname(str)
{
    if (__is_str_in_allow_chars(str, 1, "._-")) return true;
    return false;
}

function strchk_email(str)
{
    if (__is_str_in_allow_chars(str, 1, "._-@")) return true;
    return false;
}

function evtchk_url(evt)
{
    if (__is_evt_in_allow_chars(str, 1, "/.:_-?&=")) return true;
    return false;
}

function is_check_password(strpassword)
{
    var len = strpassword.length;
    var i;
    for (i=0; i<len; i++)
    {
        if (strpassword.charAt(i) == ">")
            return true;
    }
    return false;
}

function evtchk_hostname(evt)
{
    if (__is_evt_in_allow_chars(str, 1, "._-")) return true;
    return false;
}

function evtchk_email(evt)
{
    if (__is_evt_in_allow_chars(str, 1, "._-@")) return true;
    return false;
}
// about ip aera
function get_mac(m)
{
    var myMAC=new Array();
    if (m.search(":") != -1)    var tmp=m.split(":");
    else                        var tmp=m.split("-");

    for (var i=0;i <= 6;i++) myMAC[i]="";
    if (m != "")
    {
        for (var i=1;i <= tmp.length;i++) myMAC[i]=tmp[i-1];
        myMAC[0]=m;
    }
    return myMAC;
}

function get_ip(str_ip)
{
    var myIP=new Array();

    myIP[0] = myIP[1] = myIP[2] = myIP[3] = myIP[4] = "";
    if (str_ip != "")
    {
        var tmp=str_ip.split(".");
        for (var i=1;i <= tmp.length;i++) myIP[i]=tmp[i-1];
        myIP[0]=str_ip;
    }
    else
    {
        for (var i=0; i <= 4;i++) myIP[i]="";
    }
    return myIP;
}

// return netmask array according to the class of the ip address.
function generate_mask(str)
{
    var mask = new Array();
    var IP1 = decstr2int(str);

    mask[0] = "0.0.0.0";
    mask[1] = mask[2] = mask[3] = mask[4] = "0";

    if      (IP1 > 0 && IP1 < 128)
    {
        mask[0] = "255.0.0.0";
        mask[1] = "255";
    }
    else if (IP1 > 127 && IP1 < 191)
    {
        mask[0] = "255.255.0.0";
        mask[1] = "255";
        mask[2] = "255";
    }
    else
    {
        mask[0] = "255.255.255.0";
        mask[1] = "255";
        mask[2] = "255";
        mask[3] = "255";
    }
    return mask;
}

// construct a IP array
function generate_ip(str1, str2, str3, str4)
{
    var ip = new Array();

    ip[1] = (str1=="") ? "0" : decstr2int(str1.value);
    ip[2] = (str2=="") ? "0" : decstr2int(str2.value);
    ip[3] = (str3=="") ? "0" : decstr2int(str3.value);
    ip[4] = (str4=="") ? "0" : decstr2int(str4.value);
    ip[0] = ip[1]+"."+ip[2]+"."+ip[3]+"."+ip[4];
    return ip;
}

// return IP array of network ID
function get_network_id(ip, mask)
{
    var id = new Array();
    var ipaddr = get_ip(ip);
    var subnet = get_ip(mask);

    id[1] = ipaddr[1] & subnet[1];
    id[2] = ipaddr[2] & subnet[2];
    id[3] = ipaddr[3] & subnet[3];
    id[4] = ipaddr[4] & subnet[4];
    id[0] = id[1]+"."+id[2]+"."+id[3]+"."+id[4];
    return id;
}

// return IP array of host ID
function get_host_id(ip, mask)
{
    var id = new Array();
    var ipaddr = get_ip(ip);
    var subnet = get_ip(mask);

    id[1] = ipaddr[1] & (subnet[1] ^ 255);
    id[2] = ipaddr[2] & (subnet[2] ^ 255);
    id[3] = ipaddr[3] & (subnet[3] ^ 255);
    id[4] = ipaddr[4] & (subnet[4] ^ 255);
    id[0] = id[1]+"."+id[2]+"."+id[3]+"."+id[4];
    return id;
}

// return IP array of Broadcast IP address
function get_broadcast_ip(ip, mask)
{
    var id = new Array();
    var ipaddr = get_ip(ip);
    var subnet = get_ip(mask);

    id[1] = ipaddr[1] | (subnet[1] ^ 255);
    id[2] = ipaddr[2] | (subnet[2] ^ 255);
    id[3] = ipaddr[3] | (subnet[3] ^ 255);
    id[4] = ipaddr[4] | (subnet[4] ^ 255);
    id[0] = id[1]+"."+id[2]+"."+id[3]+"."+id[4];
    return id;
}

function is_valid_port_str(port)
{
    return is_in_range(port, 1, 65535);
}

// return true if the port is valid.
function is_valid_port(obj)
{
    if (is_valid_port_str(obj.value)==false)
    {
        field_focus(obj, '**');
        return false;
    }
    return true;
}

function is_valid_port_range_str(port1, port2)
{
    if (is_blank(port1)) return false;
    if (!is_valid_port_str(port1)) return false;
    if (is_blank(port2)) return true;
    if (!is_valid_port_str(port2)) return false;
    var i = parseInt(port1, [10]);
    var j = parseInt(port2, [10]);
    if (i > j) return false;
    return true;
}

// return true if the port range is valid.
function is_valid_port_range(obj1, obj2)
{
    return is_valid_port_range_str(obj1.value, obj2.value);
}

// return true if the IP address is valid.
function is_valid_ip(ipaddr, optional)
{
    var ip = get_ip(ipaddr);

    if (optional != 0)
    {
        if (ip[1]=="" && ip[2]=="" && ip[3]=="" && ip[4]=="") return true;
    }

    if (is_in_range(ip[1], 1, 223)==false) return false;
    if (decstr2int(ip[1]) == 127) return false;
    if (is_in_range(ip[2], 0, 255)==false) return false;
    if (is_in_range(ip[3], 0, 255)==false) return false;
    if (is_in_range(ip[4], 1, 254)==false) return false;

    ip[0] = parseInt(ip[1],[10])+"."+parseInt(ip[2],[10])+"."+parseInt(ip[3],[10])+"."+parseInt(ip[4],[10]);
    if (ip[0] != ipaddr) return false;

    return true;
}

// return false if the value is not a valid netmask value.
function __is_mask(str)
{
    d = decstr2int(str);
    if (d==0 || d==128 || d==192 || d==224 || d==240 || d==248 || d==252 || d==254 || d==255) return true;
    return false;
}

// return true if the netmask is valid.
function is_valid_mask(mask)
{
    var IP = get_ip(mask);

    if (__is_mask(IP[1])==false) return false;
    if (IP[1] == "0")
        return false;
    if (IP[1] != "255")
    {
        if (IP[2]=="0" && IP[3]=="0" && IP[4]=="0") return true;
        return false;
    }

    if (__is_mask(IP[2])==false) return false;
    if (IP[2] != "255")
    {
        if (IP[3]=="0" && IP[4]=="0") return true;
        return false;
    }

    if (__is_mask(IP[3])==false) return false;
    if (IP[3] != "255")
    {
        if (IP[4]=="0") return true;
        return false;
    }

    if (__is_mask(IP[4])==false) return false;
    return true;
}
// begining
function do_module_reset()
{
    var request = false;
    //alert("Module reset.");
    
    if(window.XMLHttpRequest)
    {
        request = new XMLHttpRequest();
    }
    else if(window.ActiveXObject)
    {
        request = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if(request)
    {
		request.open("GET", "/cgi-bin/mswitch.cgi?id" + Math.random(), true);
        request.send(null);
    }

    showModalDialog( '../alert.html', '12313123','center:yes;status:no;help:no;dialogWidth=430px; dialogHeight=270px');

    return;
}
function do_login_apply()
{
    var f = get_obj("login");

    if(get_obj("username").value=="")
    {
        alert("Username should not be empty. Please re-enter.");
        return;
    }
    if(get_obj("password").value=="")
    {
        alert("Password should not be empty. Please re-enter.");
        return;
    }
	
	loginbtn.disabled = true;
    document.login.submit();
    return;
}
function do_login_reset()
{
    document.login.reset();
    return;
}
// wlan
function do_wlan_apply()
{
    var f = get_obj("wlan");

	if(f.ssid.value == "")
	{
		alert("WIFI SSID should not be empty. Please re-enter.");
		return;	
	}
	
    if((f.encrytype.value == 1) || (f.encrytype.value == 2))
	{
		if(f.wifipassword.value == "")
		{
			alert("WEP password should not be empty. Please re-enter.");
			return;
		}
	}
	if(f.encrytype.value == 3)
	{
		if(f.wifipassword.value == "")
		{
			alert("WEP password should not be empty. Please re-enter.");
			return;
		}
		var str = f.wifipassword.value;
		if(str.length < 8 || str.length > 63)
		{
			alert("PSK password length must be between 8 to 63.");
			return;
		}
	}
	
	if(f.staticip.value == 1)
	{
		// check ip/subnet/gateway/dns
		if(f.staticip.value == 1)
		{
			if(f.wlansip.value == "")
			{
				alert("You have selected DHCP, Static IP address should not be empty, Please re-enter.");
				return;
			}
			if(!is_valid_ip(f.wlansip.value,1))
			{
				alert("Static IP address format error! Please re-enter.");
				f.wlansip.value = "";
				return;
			}
			if(f.wlanmip.value == "")
			{
				alert("You have selected DHCP, Subnet Mask should not be empty, Please re-enter.");
				return;
			}
			if(!is_valid_mask(f.wlanmip.value,1))
			{
				alert("Subnet Mask format error! Please re-enter.");
				f.wlanmip.value = "";
				return;
			}
			if(f.wlangip.value == "")
			{
				alert("You have selected DHCP, Default Gateway should not be empty, Please re-enter.");
				return;
			}
			if(!is_valid_ip(f.wlangip.value,1))
			{
				alert("Default Gateway format error! Please re-enter.");
				f.wlangip.value = "";
				return ;
			}
			if(f.wlandip.value == "")
			{
				alert("You have selected DHCP, DNS Server should not be empty, Please re-enter.");
				return;
			}
			if(!is_valid_ip(f.wlandip.value,1))
			{
				alert("DNS Server format error! Please re-enter.");
				f.wlandip.value = "";
				return;
			}
		}
	}

	wlanbtn.disabled = true;
    document.wlan.submit();
    return;
}
function do_wlan_reset()
{
    document.wlan.reset();
    return;
}
// wifi
function do_wifi_apply()
{
    var f = get_obj("wifi");
	
	// check port
	if(f.socketlp.value == "")
	{
		alert("Local socket port number should not be empty. Please re-enter.");
		return;
	}
	if(!is_valid_port_str(f.socketlp.value ))
	{
		f.socketlp.value = "";
		alert("Local socket port number format error! Please re-enter.");
		return;
	}
	if(f.socketrp.value == "")
	{
		alert("Local socket port number should not be empty. Please re-enter.");
		return;
	}
	if(!is_valid_port_str(f.socketrp.value ))
	{
		f.socketrp.value = "";
		alert("Local socket port number format error! Please re-enter.");
		return;
	}

	if(f.socketprol.value == 1) // udp
	{
		// check remote serverip
		if(f.socketrip.value == "")
		{
			alert("Remote server IP should not be empty. Please re-enter.");
			return;
		}
		if(!is_valid_ip(f.socketrip.value,1))
		{
			alert("Remote server IP format error! Please re-enter.");
			f.socketrip.value = "";
			return;
		}
 	}
	else // tcp
	{
		if(f.socketmode.value == 1) // client

		{
			// check remote serverip
			if(f.socketrip.value == "")
			{
				alert("Remote server IP should not be empty. Please re-enter.");
				return;
			}
			if(!is_valid_ip(f.socketrip.value,1))
			{
				alert("Remote server IP format error! Please re-enter.");
				f.socketrip.value = "";
				return;
			}
		}
		else // server
		{
			// not need to check remote serverip
		}
	}
	
	wifibtn.disabled = true;
    document.wifi.submit();
    return;
}
function do_wifi_reset()
{
    document.wifi.reset();
    return;
}
// serial
function do_serial_apply()
{
    //var f = get_obj("serial");
	
	serialbtn.disabled = true;
    document.serial.submit();
    return;
}
function do_serial_reset()
{
    document.serial.reset();
    return;
}
