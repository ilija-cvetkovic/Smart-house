var vlaznost = document.getElementById('vlaznost');
var co2 = document.getElementById('co2');
var pozar = document.getElementById('pozar');
var temp = document.getElementById('temp');
var vreme = document.getElementById('vreme');
var kisa = document.getElementById('kisa');

var ref = firebase.database().ref();
ref.on('value', snap => {
    console.log(snap.val());
    //vlaznost
    var v = snap.child('vlaznost').val();
    vlaznost.innerHTML = v;
    //co2
    var c = snap.child('co2').val();
    co2.innerHTML = c;
    //pozar
    var p = snap.child('pozar').val();
    pozar.innerHTML = p;
    //temperatura
    var t = snap.child('temp').val();
    temp.innerHTML = t + "°C";
    //kisa
    var k = snap.child('kisa').val();
    kisa.innerHTML = k;

});

function startTime() {
    var today = new Date();
    var h = today.getHours();
    var m = today.getMinutes();
    var s = today.getSeconds();
    m = checkTime(m);
    s = checkTime(s);
    document.getElementById('vreme').innerHTML =
        h + ":" + m + ":" + s;
    var t = setTimeout(startTime, 500);
    ref.update({
        '/vreme': h + ":" + m + ":" + s
    });
}

function checkTime(i) {
    if (i < 10) { i = "0" + i };
    return i;
}