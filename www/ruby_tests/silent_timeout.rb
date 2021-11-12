require 'cgi'

sleep(30)

cgi = CGI.new("html5")
cgi.out {
	cgi.html {
		cgi.head{"\n" + cgi.title{"silent timeout"} } +
		cgi.body{"\n" + 
			cgi.h1{"Silent Timeout\n"}
		}
	}
}
