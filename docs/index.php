<?php include("connection.php"); ?>
<?php
    $query = "SELECT * FROM `users` WHERE id = 1";
      $row = mysqli_fetch_array(mysqli_query($link, $query));
  $sensor1v = $row['sensor1'];
  $sensor2v = $row['sensor2'];
  $sensor3v = $row['sensor3'];
  $sensor4v = $row['sensor4'];
  $sensor5v = $row['sensor5'];
  $sensor6v = $row['sensor6'];
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Required meta tags always come first -->
    <meta charset="utf-8">
    
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta http-equiv="x-ua-compatible" content="ie=edge">
      <title>Agriculture Sensors</title>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-alpha.2/css/bootstrap.min.css" integrity="sha384-y3tfxAZXuh4HwSYylfB+J125MxIs6mR5FOHamPBG064zB+AFeWH94NdvaCBm8qnd" crossorigin="anonymous">
    <style type="text/css">

 
      .sensor,.temperature{
        width:300px;
         align-content: center;
      }
    
          
          
      html { 
          
          background: url(background.jpg) no-repeat center center fixed; 
          -webkit-background-size: cover;
          -moz-background-size: cover;
          -o-background-size: cover;
          background-size: cover;
       
    top:0;
    bottom:0;
    left:0;
    right:0;
         align-content: center;
          
          }
      
           body {
              
              background: none;
              color: #FFF;
               display: table;
                 margin: auto;
                
               top:0;
                bottom:0;
                left:0;
                right:0;
              align-content: center;
              }
          
          
      
    
    </style>
    
    
  </head>
  <body>
      
<div class="alert alert-success sensor" role="alert" id="sensor1" >
Sensor 1 <br> Recommended=<?php echo $sensor1v;?>
</div>
<div class="alert alert-success sensor" role="alert" id="sensor2" >
Sensor 2 <br> Recommended=<?php echo $sensor2v;?>
</div>
<div class="alert alert-success sensor" role="alert" id="sensor3" >
Sensor 3 <br> Recommended=<?php echo $sensor3v;?>
</div>
<div class="alert alert-success sensor" role="alert" id="sensor4" >
Sensor 4 <br> Recommended=<?php echo $sensor4v;?>
</div>
<div class="alert alert-success sensor" role="alert" id="sensor5"> 
Sensor 5 <br> Recommended=<?php echo $sensor5v;?>
</div>
<div class="alert alert-success sensor" role="alert" id="sensor6">
Sensor 6 <br> Recommended=<?php echo $sensor6v;?>
</div>
    <div class="alert alert-success sensor" role="alert" id="temperature">
      Temperature=*<strong><sup>.</sup></strong>C
</div>
        <div class="alert alert-success sensor" role="alert" id="airhumididty">
         Air Humidity=*
</div>
      <button type="button" class="btn btn-primary" id="find">Fetch</button>

    <!-- jQuery first, then Bootstrap JS. -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-alpha.2/js/bootstrap.min.js" integrity="sha384-vZ2WRJMwsjRMW/8U7i6PWi6AlO1L79snBrmgiDpgIWJ82z8eA5lenwvxbMV1PAh7" crossorigin="anonymous"></script>
      
        <script type="text/javascript">
      
       $("#find").click(function(e) {
      
        e.preventDefault();  
              
    $.ajax({
            url: "https://api.thingspeak.com/channels/623644/feeds/last.json?api_key=COIV4PHRG15EIGU2",
            type: "GET",
            success: function (data) {
                 
                
              
var sensor1vj = <?php echo json_encode($sensor1v); ?>;
var sensor2vj = <?php echo json_encode($sensor2v); ?>;
var sensor3vj = <?php echo json_encode($sensor3v); ?>;
var sensor4vj = <?php echo json_encode($sensor4v); ?>;
var sensor5vj = <?php echo json_encode($sensor5v); ?>;
var sensor6vj = <?php echo json_encode($sensor6v); ?>;
 	
    $("#sensor1").html("Sensor 1 <br> Recommended=<?php echo $sensor1v;  ?><br>Actual Value="+data["field1"]);

               	

 $("#sensor2").html("Sensor 2 <br> Recommended=<?php echo $sensor2v;  ?><br> Actual Value="+data["field2"]);

               	

 $("#sensor3").html("Sensor 3 <br> Recommended=<?php echo $sensor3v;  ?><br> Actual Value="+data["field3"]);

               	

 $("#sensor4").html("Sensor 4 <br> Recommended=<?php echo $sensor4v;  ?> <br>Actual Value="+data["field4"]);

               	

 $("#sensor5").html("Sensor 5 <br> Recommended=<?php echo $sensor5v;  ?> <br>Actual Value="+data["field5"]);

               	

 $("#sensor6").html("Sensor 6 <br> Recommended=<?php echo $sensor6v;  ?> <br>Actual Value="+data["field6"]);
 $("#temperature").html("Temperature="+data["field7"]+"<strong><sup>.</sup></strong>C");
 $("#airhumididty").html("Air Humidity="+data["field8"]);

                if(data["field1"]<sensor1vj)
               {
                    $("#sensor1").removeClass("alert-success");
                   $("#sensor1").addClass("alert-danger");
               }
                  if(data["field2"]<sensor2vj)
               {
                    $("#sensor2").removeClass("alert-success");
                   $("#sensor2").addClass("alert-danger");
               }
                  if(data["field3"]<sensor3vj)
               {
                    $("#sensor3").removeClass("alert-success");
                   $("#sensor3").addClass("alert-danger");
               }
                  if(data["field4"]<sensor4vj)
               {
                    $("#sensor4").removeClass("alert-success");
                   $("#sensor4").addClass("alert-danger");
               }
                  if(data["field5"]<sensor5vj)
               {
                    $("#sensor5").removeClass("alert-success");
                   $("#sensor5").addClass("alert-danger");
               }
                  if(data["field6"]<sensor6vj)
               {
                    $("#sensor6").removeClass("alert-success");
                   $("#sensor6").addClass("alert-danger");
               }
                  if(data["field1"]>sensor1vj)
               {
                    $("#sensor1").addClass("alert-success");
                   $("#sensor1").removeClass("alert-danger");
                }
                  if(data["field2"]>sensor2vj)
               {
                    $("#sensor2").addClass("alert-success");
                   $("#sensor2").removeClass("alert-danger");
                }
                
              if(data["field3"]>sensor3vj)
               {
                 
                    $("#sensor3").addClass("alert-success");
                   $("#sensor3").removeClass("alert-danger");
                }
                
                  if(data["field4"]>sensor4vj)
               {
                    $("#sensor4").addClass("alert-success");
                   $("#sensor4").removeClass("alert-danger");
                }
                  if(data["field5"]>sensor5vj)
               {
                    $("#sensor5").addClass("alert-success");
                   $("#sensor5").removeClass("alert-danger");
                }
                
                  if(data["field6"]>sensor6vj)
               {
                    $("#sensor6").addClass("alert-success");
                   $("#sensor6").removeClass("alert-danger");
                }
}
})
})


      
      </script>
  </body>
</html>
