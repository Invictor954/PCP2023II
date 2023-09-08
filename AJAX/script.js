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

// ... (resto del código anterior) ...

$("#obtenerClima").click(function() {
    let ciudad = $("#ciudad").val();

    if (ciudad) {
        $.ajax({
            type: 'GET',
            url: `https://api.openweathermap.org/data/2.5/weather?q=${ciudad}&appid=TU_CLAVE_DE_API&units=metric&lang=es`,
            success: function(data) {
                let infoClima = `
                    Ciudad: ${data.name} <br>
                    Temperatura: ${data.main.temp}°C <br>
                    Descripción: ${data.weather[0].description}
                `;

                $("#clima").html(infoClima);
            },
            error: function() {
                $("#clima").html("Error al obtener la información del clima, Asegurese de escribir correctamente el nombre del País");
            }
        });
    } else {
        $("#clima").html("Por favor, ingresa una ciudad.");
    }
});
