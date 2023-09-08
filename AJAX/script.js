$(document).ready(function() {
    $("#obtenerChiste").click(function() {
        $.ajax({
            type: 'GET',
            url: 'https://v2.jokeapi.dev/joke/Any',
            success: function(data) {
                let chiste = "";

                if (data.type === "single") {
                    chiste = data.joke;
                } else {
                    chiste = data.setup + "<br>" + data.delivery;
                }

                $("#chiste").html(chiste);
            },
            error: function() {
                $("#chiste").html("Error al obtener el chiste.");
            }
        });
    });
});
