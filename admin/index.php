<?php
error_reporting(0);
session_start();

if($_SESSION['berhasil'] != true){
  header('Location: ./login.php');
}

function logOut(){
  session_destroy();
  header('Location: ./login.php');
  exit;
}

?>
  
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
    <title>Blood Monitor | Admin</title>

    <!-- style & js requirements -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <link rel="stylesheet" href="./css/style.css" />
  </head>
  <body style="background-color: #f2f3f9">
    <!-- navbar -->
    <nav class="navbar navbar-expand-md navbar-dark ms-auto shadow-sm" style="background-color: rgb(206, 37, 37)">
      <div class="container">
        <a class="navbar-brand" href="#">BLOOD MONITOR</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav" aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
          <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
          <ul class="navbar-nav ms-auto">
            <li class="nav-item ms-3">
              <a class="nav-link" aria-current="page" href="#Dashboard">Dashboard</a>
            </li>
            <li class="nav-item ms-3">
              <a class="nav-link" id="setting">Setting</a>
            </li>
            <a href="./login.php" class="btn btn-light tombol ms-4" onclick="logOut()">Logout</a>
          </ul>
        </div>
      </div>
    </nav>
    <!-- end navbar -->

    <!-- cards -->
    <div class="container" id="content" style="padding-top: 3rem">
      <div class="row">
        <div class="col-md-6 mb-2">
          <div class="card bg-success shadow-lg">
            <div class="card-body">
              <div class="d-flex">
                <div class="text-white">
                  <p class="mb-0">BPM Normal</p>
                  <h2>0</h2>
                </div>
              </div>
            </div>
          </div>
        </div>
        <div class="col-md-6 mb-2">
          <div class="card bg-success shadow-lg">
            <div class="card-body">
              <div class="d-flex">
                <div class="text-white">
                  <p class="mb-0">Spo2 Normal</p>
                  <h2 id="SpoData">0</h2>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <div class="row">
        <div class="col-md-12 mb-2">
          <div class="card bg-primary shadow-lg">
            <div class="card-body">
              <div class="d-flex">
                <div class="text-white">
                  <p class="mb-0">Total Test</p>
                  <h2 id="TotalTest">0</h2>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
      <!-- end cards -->

      <!-- row -->
      <div class="row row-cards shadow-md">
        <div class="col-md-12 align-items-center">
          <div class="card">
            <div class="card-header">
              <h3 class="card-title">Data Scan</h3>
            </div>
            <div class="card-body">
              <div class="table-responsive">
                <table class="table table-hover mb-0 text-nowrap">
                  <thead>
                    <tr>
                      <th>No</th>
                      <th>UID</th>
                      <th>BPM</th>
                      <th>CO2</th>
                      <th>Time</th>
                    </tr>
                  </thead>
                  <tbody class="tableBody"></tbody>
                </table>
              </div>
            </div>
          </div>
        </div>
      </div>
      <!-- end row -->

      <!-- footer -->
      <div style="text-align: center; margin-bottom: 5px">
        Made with
        <svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" fill="red" class="bi bi-heart-fill" viewBox="0 0 16 16">
          <path fill-rule="evenodd" d="M8 1.314C12.438-3.248 23.534 4.735 8 15-7.534 4.736 3.562-3.248 8 1.314z" />
        </svg>
      </div>
      <!-- end footer -->
    </div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>
    <script type="module" src="./js/script.js"></script>
    <script type="text/javascript">
      function logOut(){
          "<?php logOut(); ?>"
        }
    </script>
  </body>
</html>
