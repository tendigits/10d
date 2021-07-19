var isb = document.querySelector('.js-imgswap');
var isi = document.querySelectorAll('.low');
if(isi.length > 0){
  isb.classList.add('util--show');    
}
var goSwap = function(el, h){
  el.setAttribute('src', h ? el.dataset.hisrc : el.dataset.lowsrc);
  el.setAttribute('width', h ? el.width*2 : el.width/2);
  el.setAttribute('height', h ? el.height*2 :el.height/2);
  el.classList.toggle('low');
  isb.innerText = h ? '- Pixels' : '+ Pixels';
}
isb.onclick = function(){
  for (var i = 0; i < isi.length; i++) {
	isi[i].classList.contains('low') ? goSwap(isi[i], true) : goSwap(isi[i], false);
  }
}