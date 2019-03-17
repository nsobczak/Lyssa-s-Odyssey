controlDivVisibilityByClass();

function controlDivVisibilityByClass() {
    var x = document.getElementsByClassName("toggleableDiv");
    for (var i = 0; i < x.length; i++) {
        console.log(x[i].id);
        if (x[i].style.display === "none") {
            x[i].style.display = "block";
        } else {
            x[i].style.display = "none";
        }
    }

    var x = document.getElementsByClassName("toggleableDivButton");
    for (var i = 0; i < x.length; i++) {
        console.log(x[i].id);
        if (x[i].innerHTML === "More") {
            x[i].innerHTML = "Hide";
        } else {
            x[i].innerHTML = "More";
        }
    }
}

function controlDivVisibility(divId, buttonId) {
    var x = document.getElementById(divId);
    if (x.style.display === "none") {
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }

    var x = document.getElementById(buttonId);
    if (x.innerHTML === "More") {
        x.innerHTML = "Hide";
    } else {
        x.innerHTML = "More";
    }
}
