<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link rel="icon" type="image/x-icon" href="/img/favicon.ico" />
    <meta name="theme-color" content="#ce2525" />
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />
    <title>Blood Monitor</title>

    <!-- style & js requirements -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous" />
    <script src="//cdn.jsdelivr.net/npm/sweetalert2@11"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <script src="js/gauge.min.js"></script>
    <link rel="stylesheet" href="./css/styles.css" />
  </head>
  <body>
    <!-- navbar -->
    <nav class="navbar navbar-expand-md navbar-dark fixed-top ms-auto shadow-sm" style="background-color: rgb(206, 37, 37)">
      <div class="container">
        <a class="navbar-brand" href="#">BLOOD MONITOR</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
          <ul class="navbar-nav ms-auto">
            <li class="nav-item ms-4">
              <a class="nav-link" aria-current="page" href="#home">Home</a>
            </li>
            <li class="nav-item ms-4">
              <a class="nav-link" href="#monitoring">Monitoring</a>
            </li>
            <li class="nav-item ms-4">
              <a class="nav-link" href="#FAQ">FAQ</a>
            </li>
            <li class="nav-item ms-4">
              <a class="nav-link">About Us</a>
            </li>
            <a href="./admin/login.php" class="btn btn-light tombol ms-4">Login</a>
          </ul>
        </div>
      </div>
    </nav>
    <!-- end navbar -->

    <!-- home -->
    <div class="container" id="home">
      <div class="row align-items-center">
        <div class="col-lg-6">
          <img src="./img/hati-min.jpg" class="img-fluid" width="700" height="700" alt="Heart Rate" />
        </div>
        <div class="col-lg-6">
          <h3>Monitor your HeartRate <br />and oximeter now</h3>
          <p>Lorem ipsum dolor sit amet consectetur adipisicing elit. Ipsa animi magnam soluta nihil eaque deserunt earum harum a ex officiis?</p>
          <a href="#monitoring" class="btn btn-primary" style="border-radius: 20px">Scan Now!</a>
        </div>
      </div>
    </div>
    <!-- end home -->

    <!-- monitoring -->
    <div class="container mb-15" id="monitoring">
      <h2 style="margin-top: 50px; margin-bottom: 80px; font-weight: bold; text-align: center">Realtime Monitor</h2>
      <div class="row justify-content-center">
        <div class="col-md-4">
          <h3>Heart rate</h3>
          <canvas id="bar1" width="350"></canvas>
          <p id="text1" style="text-align: center"></p>
        </div>
        <div class="col-md-4">
          <h3>Oximeter/Spo2</h3>
          <canvas id="bar2" width="350"> </canvas>
          <p id="text2" style="text-align: center"></p>
        </div>
      </div>
      <div class="row justify-content-center mt-10" style="padding-top: 100px">
        <div class="col-md-5">
          <div class="card" style="box-shadow: 1px 1px 1px rgba(0, 0, 0, 0.7); border-radius: 12px">
            <div class="card-body" style="margin: auto; display:block">
              <p>BPM: <span id="bpm"></span></p>
              <p>SpO2: <span id="oximeter"></span></p>
              <div class="progress">
                <div class="progress-bar" role="progressbar" style="width: 0%" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100"></div>
              </div>
              <br />
              <a href="#monitoring" class="btn btn-primary" style="width: 300px" id="btnscan">Scan!</a>
            </div>
          </div>
        </div>
      </div>
    </div>
    <!-- end monitoring -->

    <!-- FAQ -->
    <div id="FAQ">
      <div class="container" style="padding: 100px">
        <div class="row justify-content-center">
          <div class="col-lg-6">
            <h3>Apakah data kita aman?</h3>
            <p style="font-weight: 100; color: #acacac">Lorem ipsum dolor sit amet consectetur adipisicing elit. Sit ad expedita eaque rem illum unde!</p>
          </div>
          <div class="col-lg-6 py-1">
            <h3>Apakah data kita aman?</h3>
            <p style="font-weight: 100; color: #acacac">Lorem ipsum dolor sit amet consectetur adipisicing elit. Sit ad expedita eaque rem illum unde!</p>
          </div>
        </div>
        <div class="row justify-content-center">
          <div class="col-lg-6">
            <h3>Apakah data kita aman?</h3>
            <p style="font-weight: 100; color: #acacac">Lorem ipsum dolor sit amet consectetur adipisicing elit. Sit ad expedita eaque rem illum unde!</p>
          </div>
          <div class="col-lg-6 py-1">
            <h3>Apakah data kita aman?</h3>
            <p style="font-weight: 100; color: #acacac">Lorem ipsum dolor sit amet consectetur adipisicing elit. Sit ad expedita eaque rem illum unde!</p>
          </div>
        </div>
      </div>
    </div>
    <!-- end FAQ -->

    <div style="text-align: center; margin-bottom: 5px">
      Made with
      <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="red" class="bi bi-heart-fill" viewBox="0 0 16 16">
        <path fill-rule="evenodd" d="M8 1.314C12.438-3.248 23.534 4.735 8 15-7.534 4.736 3.562-3.248 8 1.314z" />
      </svg>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
    <script type="module" src="./js/script.js"></script>
  </body>
</html>
