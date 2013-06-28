
/*** INTERFACE ***/
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

function checkLoudnessBox()
{
	if( $("#loudness").is( ':checked' ) )
	{
		$(  "#ebu" ).removeAttr( "disabled" );
		$(  "#cst" ).removeAttr( "disabled" );
		$( "#atsc" ).removeAttr( "disabled" );
		$(  "#ebu-label" ).css( "color", "#fff" );
		$(  "#cst-label" ).css( "color", "#fff" );
		$( "#atsc-label" ).css( "color", "#fff" );
		$(  "#ebu-label" ).css( "cursor", "pointer" );
		$(  "#cst-label" ).css( "cursor", "pointer" );
		$( "#atsc-label" ).css( "cursor", "pointer" );
	}
	else
	{
		$(  "#ebu" ).attr( "disabled", "disabled" );
		$(  "#cst" ).attr( "disabled", "disabled" );
		$( "#atsc" ).attr( "disabled", "disabled" );
		$(  "#ebu" ).removeAttr( "checked" );
		$(  "#cst" ).removeAttr( "checked" );
		$( "#atsc" ).removeAttr( "checked" );
		$(  "#ebu-label" ).css( "color", "#444" );
		$(  "#cst-label" ).css( "color", "#444" );
		$( "#atsc-label" ).css( "color", "#444" );
		$(  "#ebu-label" ).css( "cursor", "default" );
		$(  "#cst-label" ).css( "cursor", "default" );
		$( "#atsc-label" ).css( "cursor", "default" );
	}
}

function checkEmailBox()
{
	if( $("#send-mail").is( ':checked' ) )
	{
		$(    "#inputMail" ).removeAttr( "disabled" );
		$(     "#pdf-mail" ).removeAttr( "disabled" );
		$(    "#html-mail" ).removeAttr( "disabled" );
		$( "#html-display" ).removeAttr( "disabled" );
		$(  "#format-mail-label" ).css( "color", "#fff" );
		$(     "#pdf-mail-label" ).css( "color", "#fff" );
		$(    "#html-mail-label" ).css( "color", "#fff" );
		$( "#html-display-label" ).css( "color", "#fff" );
		$(  "#format-mail-label" ).css( "cursor", "pointer" );
		$(     "#pdf-mail-label" ).css( "cursor", "pointer" );
		$(    "#html-mail-label" ).css( "cursor", "pointer" );
		$( "#html-display-label" ).css( "cursor", "pointer" );
		$(          "#inputMail" ).css( "cursor", "text" );
	}
	else
	{
		$(    "#inputMail" ).attr( "disabled", "disabled" );
		$(     "#pdf-mail" ).attr( "disabled", "disabled" );
		$(    "#html-mail" ).attr( "disabled", "disabled" );
		$( "#html-display" ).attr( "disabled", "disabled" );
		$(  "#format-mail-label" ).css( "color", "#444" );
		$(     "#pdf-mail-label" ).css( "color", "#444" );
		$(    "#html-mail-label" ).css( "color", "#444" );
		$( "#html-display-label" ).css( "color", "#444" );
		$(  "#format-mail-label" ).css( "cursor", "default" );
		$(     "#pdf-mail-label" ).css( "cursor", "default" );
		$(    "#html-mail-label" ).css( "cursor", "default" );
		$( "#html-display-label" ).css( "cursor", "default" );
		$(          "#inputMail" ).css( "cursor", "default" );
	}
}

$( document ).ready( )
{
	console.log( "Ready !" );
	checkEmailBox()
	checkLoudnessBox()
	if( typeof( $(".accordion input").css( "display" ) ) != "undefined" )
		displayCharts();
}

$("#loudness").click( function() {
	console.log( "Loudness !" );
	checkLoudnessBox()
});

$("#send-mail").click( function() {
	console.log( "Email !" );
	checkEmailBox()
});

/*** PROCESS ***/
function getStart()
{
	var now = new Date();
	start = now.getDate() + "/" + now.getMonth() + "/" + now.getFullYear();
	start += ", at ";
	start += now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds(); // + "." + now.getMilliseconds();
	return start;
}

