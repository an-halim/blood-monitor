<?php
error_reporting(0);
session_start();
if($_SESSION['berhasil'] == true){
	header("Location: ./index.php");
}else{
	if(isset($_POST['login'])){
		$user = $_POST['username'];
		$pass = $_POST['password'];
		if ($user == 'admin' AND $pass == 'demo'){
			$_SESSION['berhasil'] = true;
			header("Location: ./index.php");
		} else {
			$salah = "<p> username dan password salah </p>";
		}
	}
}
?>

<!DOCTYPE html>
<html>	
<head>
	<title>Blood Monitor | Login</title>
	<link rel="stylesheet" type="text/css" href="login/style.css">
	<link href="https://fonts.googleapis.com/css?family=Poppins:600&display=swap" rel="stylesheet">
	<script src="https://kit.fontawesome.com/a81368914c.js"></script>
	<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
	<img class="wave" src="./login/img/wave.png">
	<div class="container">
		<div class="img">
			<img src="./login/img/hati.png">
		</div>
		<div class="login-content">
			<form action="" method="post">
				<img src="./login/img/avatar.svg">
				<h2 class="title">Log In</h2>
				<?php echo $salah; ?>
           		<div class="input-div one">
           		   <div class="i">
           		   		<i class="fas fa-user"></i>
           		   </div>
           		   <div class="div">
           		   		<h5>Username</h5>
           		   		<input type="text" name="username" class="input">
           		   </div>
           		</div>
           		<div class="input-div pass">
           		   <div class="i"> 
           		    	<i class="fas fa-lock"></i>
           		   </div>
           		   <div class="div">
           		    	<h5>Password</h5>
           		    	<input type="password" name="password" class="input">
            	   </div>
            	</div>
            	<button type="submit" name="login" class="btn">Submit</button>
            </form>
        </div>
    </div>
    <script type="text/javascript" src="login/main.js"></script>
</body>
</html>
