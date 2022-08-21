$(document).ready(function(){
defaultTxt = $(".es-actionTextArea").val();
$(".es-actionTextArea").focus(function(){
if(this.value == defaultTxt){
jQuery(this).val("");
}
});
$(".es-actionTextArea").blur(function(){
if(jQuery.trim(this.value) == ""){
jQuery(this).val(defaultTxt);
}
});
$(".es-actionPreview").hide();
$("#actionSubmit").click(function(){
$(".es-actionPreview").show("slow"); 
});
});