require 'cgi'

cgi = CGI.new("html5")
cgi.out {
	cgi.html {
		cgi.head{"\n" + cgi.title{"POST FORM"} } +
		cgi.body{"\n" +
		cgi.img("./www/home.jpg", "return home", 20, 20) + 
		cgi.a("http://localhost:8080") {"Return to localhost"} +
		cgi.form{"\n" +
			cgi.hr +
			cgi.h1 {"A Form: "} + "\n" +
			cgi.textarea{"Send me to the server"} + "\n" +
			cgi.br +
			cgi.submit
		}
		}
	}
}
