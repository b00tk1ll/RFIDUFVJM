
(function(){
    
  // Inicia o firebase Firebase
  var firebaseConfig = {
    apiKey: "AIzaSyDxRy6v71CBnPaFda4tc9YcZ7IKmUnCpUg",
    authDomain: "rfidufvjm.firebaseapp.com",
    databaseURL: "https://rfidufvjm.firebaseio.com",
    projectId: "rfidufvjm",
    storageBucket: "rfidufvjm.appspot.com",
    messagingSenderId: "428823677512",
    appId: "1:428823677512:web:c616c9298b94e0dd"
  };
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);


  var db = firebase.database();

  // Cria os listeners dos dados no firebase
  var presenceRef = db.ref('presence');
  var btnRef = db.ref('btn');
  var cardRef = db.ref('card');


  // Registrar função ao alterar valor de presença
  presenceRef.on('value', function(snapshot){
    var value = snapshot.val();
    var el = document.getElementById('currentPresence')
    if(value){
      document.getElementById('entrou').innerHTML='Você está dentro da sala';
      el.classList.add('green-text');
    }else{
      document.getElementById('entrou').innerHTML='Você não está na sala';
      el.classList.remove('green-text');      
    }
  });

  // Registrar função ao alterar valor da btnada
  var currentBtnValue = false;
  btnRef.on('value', function(snapshot){
    var value = snapshot.val();
    var el = document.getElementById('currentBtn')
    if(value){
      document.getElementById('btn-btn').innerHTML='Sair';
    }else{
      document.getElementById('btn-btn').innerHTML='Entrar';
    }
    currentBtnValue = !!value;
  });


  var currentCardValue = false;
  cardRef.on('value', function(snapshot){
    var value = snapshot.val();
    var el = document.getElementById('currentBtn')
    if(value){
      el.classList.add('blue-text');
      document.getElementById('pass_card').innerHTML='Passe o cartão';
    }else{
      el.classList.remove('blue-text');
      document.getElementById('pass_card').innerHTML='';
    }
    currentCardValue = !!value;
  });

  // Registrar função de click no botão de btnada
  var btnBtn = document.getElementById('btn-btn');
  btnBtn.addEventListener('click', function(evt){
    cardRef.set(1);
  });

})();


// Retorna uma função que de acordo com as mudanças dos dados
// Atualiza o valor atual do elemento, com a metrica passada (currentValueEl e metric)
// e monta o gráfico com os dados e descrição do tipo de dados (chartEl, label)
function onNewData(currentValueEl, chartEl, label, metric){
  return function(snapshot){
    var readings = snapshot.val();
    if(readings){
        var currentValue;
        var data = [];
        for(var key in readings){
          currentValue = readings[key]
          data.push(currentValue);
        }

        document.getElementById(currentValueEl).innerText = currentValue + ' ' + metric;
        buildLineChart(chartEl, label, data);
    }
  }
}