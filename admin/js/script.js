window.onload = $.getJSON( "https://database-mikro-default-rtdb.firebaseio.com/.json", function( data ) {
  var table = $("#table").DataTable();
  var js = JSON.stringify(data.database.data);
  var json = JSON.parse(js)
  var totalTest = 0;  
  var totalSpo = 0;
  for (var key in json) {
    console.log(`${key} => BPM ${json[key].bpm} => Spo2 ${json[key].spo2}`)    
    var dataset = [`${i + 1}`, `${json[key].name}`, `${json[key].bpm}`, `${json[key].spo2}%`, `${json[key].time}`];
    table.rows.add([dataset]).draw();
    if (json[key].spo2 > 85) {
      totalSpo++;
    }
    totalTest++;
  }
  document.getElementById("SpoData").innerHTML = totalSpo
  document.getElementById("TotalTest").innerHTML = totalTest;
  console.log(`Total ${totalTest}`)
});

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function load() {
  $.getJSON( "https://database-mikro-default-rtdb.firebaseio.com/.json", function( data ) {
    var table = $("#table").DataTable();
    var js = JSON.stringify(data.database.data);
    var json = JSON.parse(js)
    var i = 0;  
    var total = 0;
    var totalSpo = 0;
    for (var key in json) {
      total++;
    }
    if(total > parseInt(document.getElementById("TotalTest").innerHTML) || total < parseInt(document.getElementById("TotalTest").innerHTML)){      
      table.clear().draw();
      sleep(200)
      for (var key in json) {
        console.log(`${key} => BPM ${json[key].bpm} => Spo2 ${json[key].spo2}`)    
        var dataset = [`${i + 1}`, `${json[key].name}`, `${json[key].bpm}`, `${json[key].spo2}%`, `${json[key].time}`];
        table.rows.add([dataset]).draw();
        if (json[key].spo2 > 85) {
          totalSpo++;
        }
        i++;
      }
      document.getElementById("SpoData").innerHTML = totalSpo
      document.getElementById("TotalTest").innerHTML = i;
      console.log(`Total ${i}`)
    }
  
 });
}

setInterval(function () {
  console.log("Tes interval")
  load();
}, 1000);
