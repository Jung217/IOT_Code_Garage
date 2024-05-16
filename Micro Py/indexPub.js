var mqtt = require('mqtt');
var client = mqtt.connect({ host: '192.168.50.248', port: 1883 });

client.on('connect',function(){
  console.log("publish connect broker");
});
// client.on('connect',function(){
//   console.log("connecting")
//   client.subscribe("test1")
//   client.subscribe("second")
// })
// client.on('message',function(topic,message){
//    console.log(topic,message.toString())
// })
var i=0;
setInterval(function(){
  /*var text='successful: ';
  
  text+=i.toString();
  console.log("test: ",text );
  client.publish('test', text);*/
  var pubb = i % 10;
  var pub = pubb.toString();
  client.publish('test', pub);
  console.log(pub);
  i++;
}, 5000);



