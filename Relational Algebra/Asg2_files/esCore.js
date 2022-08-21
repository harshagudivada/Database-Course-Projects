if(typeof(Edushare) == "undefined") Edushare = {};
if(typeof(Edushare.FunctionalTextFields) == "undefined") Edushare.FunctionalTextFields = {}; 
if(typeof(Edushare.Dialogs) == "undefined") Edushare.Dialogs = {}; 

if(typeof(Wicket) == "undefined") Wicket = {};
if(typeof(Wicket.Form) == "undefined") Wicket.Form = {};

Edushare.FunctionalTextFields.MarkField = function(target){
	$(target).data('marked','marked');
};

Edushare.FunctionalTextFields.isFieldMarked = function(target){
	if($(target).data('marked') != "marked")
		return new Boolean(false);
	else
		return new Boolean(true);
};

Edushare.FunctionalTextFields.activate = function(target){
	if(Edushare.FunctionalTextFields.isFieldMarked(target) == false){
		Edushare.FunctionalTextFields.MarkField(target);
		$(target).val("");
	}
};

Edushare.FunctionalTextFields.SetError = function(errorMessage){
	$(".errStr").html(errorMessage);
	$(".errorMsg").slideDown("slow");
};

Edushare.FunctionalTextFields.ClearError = function(){
	$(".errStr").text('');
	$(".errorMsg").hide();
};

Edushare.Dialogs.confirmDialog = function(id){
	$('#'+id).dialog({
        autoOpen: false,
        width: 600,
        buttons: {
            "Ok": function() { 
                return true;
            }, 
            "Cancel": function() { 
                return false; 
            } 
        }
    });

}


$(document).ready(function(){
	
	/** Error Slide-in box **/
	jQuery.fn.center = function () {
		this.css("position","fixed");
		this.css("top", "0px");
		this.css("left", ( $(window).width() - this.width() ) / 2+$(window).scrollLeft() + "px");
		return this;
	};
	
	$(".errorMsg").center();
	
	$("#closeErrMsg").click(function(){
		$(".errorMsg").slideUp();							 
	});
	
	if(jQuery.trim($(".errStr").text())==''){
		$(".errorMsg").hide();
	}

	/**Hover highlight effect on buttons */
    $(".es-button").hover(
        function() { $(this).addClass('ui-state-hover'); }, 
        function() { $(this).removeClass('ui-state-hover'); }
    );
	
	/**Hover highlight effect on FB-tabs */
	$(".es-fbTabItem").hover(
		function() { $(this).addClass('es-fbTabHover'); }, 
        function() { $(this).removeClass('es-fbTabHover'); }				 
	);
	
	$('.initialTextInputField').each(function(){
		var helpText = $(this).attr('initialText');
		$(this).val(helpText);
		var target = this;
		$(this).focus(function() {
			Edushare.FunctionalTextFields.activate(target);
		});
		$(this).click(function(){
			Edushare.FunctionalTextFields.activate(target);
		});
	});
	
	/** Soft corners effect on Block elements */
	$(".es-softCornerTL").corner("tl");
	$(".es-softCornerTR").corner("tr");
	$(".es-softCornerBL").corner("bl");
	$(".es-softCornerBR").corner("br");
	$(".es-softCornerTop").corner("top");
	$(".es-softCornerRight").corner("right");
	$(".es-softCornerBottom").corner("bottom");
	$(".es-softCornerLeft").corner("left");
	$(".es-softCornerAll").corner();
	
	/**show menus marked as menu-able as children of tag with class name es-hoverMenu*/
	$(".es-hoverMenu").hover(
		function() { $(this).find('.es-menuable').show(); }, 
        function() { $(this).find('.es-menuable').hide(); }
	);
	
	/** Zebra pattern for tables */
	$(".stripes tr").mouseover(function(){
        $(this).addClass("over");
    });
    $(".stripes tr").mouseout(function(){
        $(this).removeClass("over");
    });
    $(".stripes tr:even").addClass("alt");
	
    /** External anchors open in new window */
    /*$("a.external").attr("target","_blank");
    $("a.external").click(function(){
    	window.open(this.href);
    	return false;
    });*/
    
    /** helpTip */
    $('.helpTip').each(function(){
        $(this).qtip({
            content: $(this).next('div').html(),
            style:{
                name:'dark',
                tip:true
            },
            position:{
                corner:{
                    tooltip:'bottomLeft',
                    target:'topRight'
                }
            },
            position:{ 
            	adjust:{
            		screen: true
            	} 
            }
        });
        
    });

    
    /*
    $(".helpTipT").tooltip({
	   	
	 	   effect: "fade",
	 	   position: "top center",
	 	   offset: "[-20, 0]"
	 });
 
    $(".helpTipR").tooltip({
		   	
	 	   effect: "fade",
	 	   position: "center right",
	 	   offset: "[0, 20]"
	 });

    
   $(".helpTipB").tooltip({
	   	
    	   effect: "fade",
    	   position: "bottom center",
    	   offset: "[20, 0]"
    });
   
   $(".helpTipL").tooltip({
	   	
		   effect: "fade",
		   position: "center left",
		   offset: "[0, 20]"
	});
    
   $(".helpTip").tooltip();
   */
    
});