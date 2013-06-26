$('#inputFile').keyup(function() {
	$.get("ls", {
		indir: $('#inputFile').val() 
	}).done(function(data) {
		// console.log( data );
		$( '#inputFile' ).autocomplete({
			source: data
		});
	});
});

$("#inputCheck").click( function() {
	var now = new Date();
	start = now.getDate() + "/" + now.getMonth() + "/" + now.getFullYear();
	start += ", at ";
	start += now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds(); // + "." + now.getMilliseconds();
	$.ajax({
		type: "POST",
		url: "execute",
		data: { filename: $('#inputFile').val(), email: $('#inputMail').val(), date: start }
	}).done( function ( msg ) {
		if( msg )
		{
			$('#container').css( "display", "block" );
			$('#date').html( "<a>Started on : " + start + ".</a>" );
			$('#date').css( "display", "block" );
			$('#container').html( msg );
			$('#fake').click();
		}
	});
});
