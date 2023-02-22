source.addEventListener('RX_Values', function(e) {
    console.log("RX_Values", e.data);
    var obj = JSON.parse(e.data);
    var Channels = obj.Channels
    document.getElementById("ThrottleGraph").style.width = obj.THROTTLEP + '%';
    document.getElementById("ThrottleGraph").innerHTML = "Throttle: " + obj.THROTTLE;
    document.getElementById("SteeringGraph").style.width = obj.STEERINGP +'%';
    document.getElementById("SteeringGraph").innerHTML = "Steering: " + obj.STEERING;
    if(Channels > 2) {
      document.getElementById("Ch3Graph").style.width = obj.CHAN3P + '%';
      document.getElementById("Ch3Graph").innerHTML = "Channel 3: " + obj.CHAN3;
    }
    if(Channels > 3) {
      document.getElementById("Ch4Graph").style.width = obj.CHAN4P + '%';
      document.getElementById("Ch4Graph").innerHTML = "Channel 4: " + obj.CHAN4;
    }
    if(Channels > 4) {
      document.getElementById("Ch5Graph").style.width = obj.CHAN5P + '%';
      document.getElementById("Ch5Graph").innerHTML = "Channel 5: " + obj.CHAN5;
    }
    if(Channels > 5) {
      document.getElementById("Ch6Graph").style.width = obj.CHAN6P + '%';
      document.getElementById("Ch6Graph").innerHTML = "Channel 6: " + obj.CHAN6;
    }
    if(Channels > 6) {
      document.getElementById("Ch7Graph").style.width = obj.CHAN7P + '%';
      document.getElementById("Ch7Graph").innerHTML = "Channel 7: " + obj.CHAN7;
    }
    if(Channels > 7) {
      document.getElementById("Ch8Graph").style.width = obj.CHAN8P + '%';
      document.getElementById("Ch8Graph").innerHTML = "Channel 8: " + obj.CHAN8;
    }
  }, false);