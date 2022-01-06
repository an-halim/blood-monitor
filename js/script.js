import { initializeApp } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-app.js";
import { getDatabase, ref, onValue, child, get, set, update } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-database.js";
// let config = $.getJSON("config.json", function (data) {
//   return data;
// });

// console.log(JSON.stringify(config));

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

// responsive gauge
function circle(id, value, max) {
  var opts = {
    angle: 0.01, // The span of the gauge arc
    lineWidth: 0.25, // The line thickness
    radiusScale: 0.85, // Relative radius
    pointer: {
      length: 0.5, // // Relative to gauge radius
      strokeWidth: 0.03, // The thickness
      color: "#000000", // Fill color
    },
    staticLabels: {
      font: "10px sans-serif",
      labels: [30, 60, 90, max],
      fractionDigits: 0,
    },
    staticZones: [
      { strokeStyle: "#F03E3E", min: 0, max: 30 },
      { strokeStyle: "#F03E3E", min: 30, max: 60 },
      { strokeStyle: "#FFDD00", min: 60, max: 85 },
      { strokeStyle: "#30B32D", min: 85, max: 90 },
      { strokeStyle: "#30B32D", min: 90, max: max },
      //{ strokeStyle: "#F03E3E", min: 2800, max: 3000 },
    ],
    limitMax: false, // If false, max value increases automatically if value > maxValue
    limitMin: false, // If true, the min value of the gauge will be fixed
    colorStart: "#6FADCF", // Colors
    colorStop: "#8FC0DA", // just experiment with them
    strokeColor: "#E0E0E0", // to see which ones work best for you
    generateGradient: true,
    highDpiSupport: true, // High resolution support
  };
  var target = document.getElementById(id); // your canvas element
  var gauge = new Gauge(target).setOptions(opts); // create sexy gauge!
  gauge.maxValue = max; // set max gauge value
  gauge.setMinValue(0); // Prefer setter over gauge.minValue = 0
  gauge.animationSpeed = 5; // set animation speed (32 is default value)
  gauge.set(value); // set actual value
}

function getLastdata() {
  var bpm = document.getElementById("bpm");
  var co2 = document.getElementById("oximeter");

  const dbRef = ref(getDatabase());
  get(child(dbRef, `database/`))
    .then((snapshot) => {
      if (snapshot.exists()) {
        var lasData = snapshot.val().total - 1;
        console.log(snapshot.val().data[lasData]);
        bpm.innerHTML = snapshot.val().data[lasData]["bpm"];
        co2.innerHTML = snapshot.val().data[lasData]["spo2"];
        var rate = snapshot.val().data[lasData]["bpm"];
        var spo2 = snapshot.val().data[lasData]["spo2"];
        document.getElementById("text1").innerHTML = rate.toString();
        document.getElementById("text2").innerHTML = spo2.toString();

        circle("bar1", rate, 220);
        circle("bar2", spo2, 100);
      } else {
        console.log("No data available");
      }
    })
    .catch((error) => {
      console.error(error);
    });
}

function updateState(state) {
  const db = getDatabase();
  update(ref(db, "database/"), {
    state: state,
  });
}

function AddData() {
  const dbRef = ref(getDatabase());
  get(child(dbRef, `database/`))
    .then((snapshot) => {
      if (snapshot.exists()) {
        let lasData = snapshot.val().total;
        const db = getDatabase();
        update(ref(db, "database/data/" + lasData), {
          uid: "FA FR FA HD YD",
          rate: 34,
          co2: rand(50, 100).toString() + "%",
          time: new Date().toLocaleString(),
        });
        update(ref(db, "database/"), {
          total: lasData + 1,
        });
      } else {
        console.log("No data available");
      }
    })
    .catch((error) => {
      console.error(error);
    });
}

function getState() {
  let state = 0;
  const dbRef = ref(getDatabase());
  get(child(dbRef, `database/`))
    .then((snapshot) => {
      state += snapshot.val().state;
    })
    .catch((error) => {
      console.error(error);
    });
  return state;
}

function clear() {
  var bpm = document.getElementById("bpm");
  var co2 = document.getElementById("oximeter");
  bpm.innerHTML = " ";
  co2.innerHTML = " ";
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

if (getState() == 1) {
  console.log("test");
}

circle("bar1", 0, 220);
circle("bar2", 0, 100);

function rand(min, max) {
  return Math.floor(Math.random() * (max - min) + min);
}

$("#btnscan").click(async function () {
  $(this).addClass("disabled");
  clear();
  updateState(1);
  $("#progress").show();
  var updateGauge = setInterval(function () {
    getLastdata();
  }, 1000);

  for (let index = 0; index <= 100; index++) {
    $(".progress-bar").css("width", index + "%");
    await sleep(750);
  }
  updateState(0);
  $(this).removeClass("disabled");
  clearInterval(updateGauge);
  Swal.fire("Successfully!", "You have scan your heart rate!", "success");
  console.log(getLastdata());
});

$("#btnadd").click(function () {
  AddData();
  Swal.fire({
    title: "You have add new data!",
    icon: "success",
    confirmButtonColor: "#058ae3",
  });
});
