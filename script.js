// Countdown timer
var countDownDate = new Date("Jun 12, 2024 00:00:00").getTime();


var x = setInterval(function() {
  var now = new Date().getTime();
  var distance = countDownDate - now;
  var days = Math.floor(distance / (1000 * 60 * 60 * 24));
  var hours = Math.floor((distance % (1000 * 60 * 60 * 24)) / (1000 * 60 * 60));
  var minutes = Math.floor((distance % (1000 * 60 * 60)) / (1000 * 60));
  var seconds = Math.floor((distance % (1000 * 60)) / 1000);
  document.getElementById("countdown").innerHTML = days + "d " + hours + "h "
  + minutes + "m " + seconds + "s ";
  if (distance < 0) {
    clearInterval(x);
    document.getElementById("countdown").innerHTML = "EXPIRED";
  }
}, 1000);

// Ruimtenieuwsfeed
var newsFeed = document.getElementById("news-feed");
var newsItems = [
  "NASA kondigt plannen aan voor AASA Maanlander-missie",
  "Europese ruimtevaartorganisatie lanceert nieuwe satelliet voor aarde observatie",
  "Ruimtetoerisme bedrijf AAAD onthult plannen voor de maanlander"
];

newsItems.forEach(function(item) {
  var li = document.createElement("li");
  li.textContent = item;
  newsFeed.appendChild(li);
});

function uitklappen() {
  console.log("hallo ik doe de uit klap");
}

function inklappen() {
  console.log("hoi ik doe de inklap");
}

function artikel() {

  document.getElementById('artikel').innerHTML = "Aan het schrijven... Heb geduld!"
  const AWANLLM_API_KEY = "3f8b5e11-562c-41a5-aaa8-03e48eb52ade"; // Replace "your_api_key_here" with your actual API key

  fetch("https://api.awanllm.com/v1/chat/completions", {
    method: "POST",
    headers: {
      "Authorization": `Bearer ${AWANLLM_API_KEY}`,
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      "model": "Meta-Llama-3-8B-Instruct",
      "messages": [
        {"role": "user", "content": "antwoord in het nederlands. Schrijf een nepartikel over hoe de Avans Academie Associate Degree informatica en Engeniering samen een maanlander hebben gemaakt die op 12 juni op de maan gaat landen."}
      ]
    })
  })
  .then(response => response.json())
  .then(data => {
    console.log(data.choices[0].message.content); // You can handle the response data here
    document.getElementById("artikel").innerHTML = data.choices[0].message.content;
  })
  .catch(error => {
    console.error('Error:', error);
  });

}
function test() {
  setSpanningStroom2(Math.round((Math.random() * 5.0) * 10) / 10, Math.round((Math.random() * 0.1) * 1000) / 1000, 
                     Math.round((Math.random() * 5.0) * 10) / 10, Math.round((Math.random() * 0.1) * 1000) / 1000);

  const button = document.getElementById('spintest');
  button.classList.add('spin');

  // Remove the 'spin' class after the animation completes
  setTimeout(() => {
    button.classList.remove('spin');
  }, 1000);

}

function setSpanningStroom2(BaSp, BaSt, ZoSp, ZoSt) {
var table = document.getElementById("data table");

var lastRow = table.rows[table.rows.length - 1];
table.deleteRow(lastRow.rowIndex);


var rows = table.getElementsByTagName("tr");
for (var i = 2; i < rows.length; i++) { 
    rows[i].classList.add("oud");
}

var newRow = table.insertRow(2); 

var TijdCell = newRow.insertCell(0);
var BaStroomCell = newRow.insertCell(1);
var BaSpanningCell = newRow.insertCell(2);
var ZoStroomCell = newRow.insertCell(3);
var ZoSpanningCell = newRow.insertCell(4);

var d = new Date();

TijdCell.innerHTML = d.toLocaleString(); 
BaStroomCell.innerHTML = BaSt.toString() + " A";
BaSpanningCell.innerHTML = BaSp.toString() + " V";
ZoStroomCell.innerHTML = ZoSt.toString() + " A";
ZoSpanningCell.innerHTML = ZoSp.toString() + " V";
}


function setSpanningStroomPercentage(datumTijd, ZoSt, ZoSp, BaSt, BaSp, percentage) {
  var newRow = table.insertRow();
  var datumTijdCell = newRow.insertCell(0);
  var ZoStCell = newRow.insertCell(1);
  var ZoSpCell = newRow.insertCell(2);
  var BaStCell = newRow.insertCell(3);
  var BaSpCell = newRow.insertCell(4);
  var percentageCell = newRow.insertCell(5);
}

function testSend(positie_paneel) {
  fetch('http://145.49.127.247:1880/aaadlander/aaad1?positie_paneel=' + positie_paneel, {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json'
      },
      body: JSON.stringify({ positie_paneel: positie_paneel })
  })
  .then(response => response.json())
  .then(data => {
      console.log('Success:', data);
  })
  .catch((error) => {
      console.error('Error:', error);
  });
}


let socket = new WebSocket("ws://145.49.127.247:1880/ws/aaad1");

socket.addEventListener("message", (event => {
  console.log(event.data);
}))

socket.onmessage = function(event) {
  let message = event.data;

  console.log(message);
}