var mqtt = require('mqtt');
var client = mqtt.connect({ host: '192.168.50.248', port: 1883 });
var i = 0;
console.log("first");
client.on('connect', function () {
  console.log("subscribe connect broker");
})
client.subscribe('test');
console.log("subscribed test");
// console.log("subscribe2")
// setInterval(function(){
// var r=Math.floor(Math.random()*100).toString()

// console.log('test1',r)
// client.publish('test1','welcome'+r)

// },2000)
client.on('message', function (topic, message) {
  data = message.toString();
  console.log(topic,data,typeof(data),i);
  i += 1;
});

