function hideUnhideScope(id) {
    var domid = document.getElementById(id);

    if (domid.style.display == 'none'){
        domid.style.display = 'block';
	}
    else {
        domid.style.display = 'none';
	}
}