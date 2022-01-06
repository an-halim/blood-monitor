import { initializeApp } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-app.js";
import { getDatabase, ref, onValue, child, get, set, update } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyAfV5akMqmGqFSuO49LX8k06V3NCwzJK7Q",

  authDomain: "database-mikro.firebaseapp.com",

  databaseURL: "https://database-mikro-default-rtdb.firebaseio.com",

  projectId: "database-mikro",

  storageBucket: "database-mikro.appspot.com",

  messagingSenderId: "646869111765",

  appId: "1:646869111765:web:f0e1d48f2082c8f5c5235c",
};

initializeApp(firebaseConfig);

let lasData = 0;

window.onload = load();

function load() {
  var table = $("#table").DataTable();
  const dbRef = ref(getDatabase());
  get(child(dbRef, `database/`))
    .then((snapshot) => {
      console.log(snapshot.val().data);
      for (let index = 0; index < snapshot.val().total; index++) {
        var dataset = [`${index + 1}`, `${snapshot.val().data[index]["uid"]}`, `${snapshot.val().data[index]["bpm"]}`, `${snapshot.val().data[index]["spo2"]}%`, `${snapshot.val().data[index]["time"]}`];
        table.rows.add([dataset]).draw();
        if (snapshot.val().data[index]["spo2"] > 85) {
          document.getElementById("SpoData").innerHTML = parseInt(document.getElementById("SpoData").innerHTML) + 1;
        }
      }
      document.getElementById("TotalTest").innerHTML = snapshot.val().total;
      lasData = snapshot.val().total;
    })
    .catch((error) => {
      console.error(error);
    });
}

function updateData() {
  const dbRef = ref(getDatabase());
  get(child(dbRef, `database/`))
    .then((snapshot) => {
      if (lasData < snapshot.val().total) {
        let no = lasData;
        for (let index = lasData; index < snapshot.val().total; index++) {
          no++;
          $(".tableBody").append(`<tr id="tabelContent">`);
          $(".tableBody").append(`<th id="tabelContent">${no}</th>`);
          $(".tableBody").append(`<td id="tabelContent">${snapshot.val().data[index]["uid"]}</td>`);
          $(".tableBody").append(`<td id="tabelContent">${snapshot.val().data[index]["bpm"]}</td>`);
          $(".tableBody").append(`<td id="tabelContent">${snapshot.val().data[index]["spo2"]}%</td>`);
          $(".tableBody").append(`<td id="tabelContent">${snapshot.val().data[index]["time"]}</td>`);
          $(".tableBody").append(`</tr>`);
          /*if (parseInt(snapshot.val().data[index]["spo2"].replace("%")) > 85) {
            document.getElementById("SpoData").innerHTML = parseInt(document.getElementById("SpoData").innerHTML) + 1;
          }*/
        }
        document.getElementById("TotalTest").innerHTML = snapshot.val().total;
        lasData = snapshot.val().total;
      } else if (lasData > snapshot.val().total) {
        $(".tableBody").remove();
        $(".table").append(`<tbody class="tableBody"></tbody>`);
        load();
      }
    })
    .catch((error) => {
      console.error(error);
    });
}

setInterval(function () {
  updateData();
}, 200);

$("#setting").click(function () {
  $("#content").load("./setting.php");
});
