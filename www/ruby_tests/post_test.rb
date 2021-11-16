require 'cgi'

cgi = CGI.new("html5")
cgi.out {
	cgi.html {
		cgi.head{"\n" + cgi.title{"POST FORM"} } +
		cgi.body{"\n" +
		cgi.img("./www/home.jpg", "return home", 20, 20) + 
		cgi.a("http://localhost:8080") {"Return to localhost"} +
		cgi.form("ENCTYPE" => "multipart/form-data"){"\n" +
			cgi.p {
			cgi.hr +
			cgi.h1 {"A Form: "} + "\n" +
			cgi.text_field("NAME" => "toSend"){"toSend"} + "\n" +
			cgi.file_field("NAME" => "file") +
			cgi.br +
			cgi.submit +
			cgi.br
		}}
		} +
		cgi.pre {
		#	params = cgi.params['toSend']
			CGI.escapeHTML("params: #{cgi.params.inspect}\n" +
			ENV.collect do |key, value|
			 "#{key} --> #{value}\n"
			end.join("")
		)
		}
	}
}
