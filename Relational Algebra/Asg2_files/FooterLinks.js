$(document).ready(function(){
$("#ufeedbackall").attr('style','display:none;');
$('#closeufb').live('click',function(){
$("#ufeedbackall").attr('style','display:none;');
return false;
});
$('[id=^reportBug]').click(function(){
$('#ufeedbackall').show();
return false;
});
});