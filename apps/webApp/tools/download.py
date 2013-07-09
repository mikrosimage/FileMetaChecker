import cherrypy
from cherrypy.lib.static import serve_file

class Download:
	def __init__( self, filename ):
		self.filename = filename + ".pdf"

	@cherrypy.expose
	def index(self, filepath ):
		return serve_file( filepath, "application/x-download", "attachment", self.filename )
