(function(){/* 
 
 Copyright The Closure Library Authors. 
 SPDX-License-Identifier: Apache-2.0 
*/ 
var b=document;function g(){for(var c=b.head,e=c.querySelectorAll("link[data-reload-stylesheet][as=style][rel=preload]"),d=0;d<e.length;d++){var h=e[d],a="link",f=document;a=String(a);"application/xhtml+xml"===f.contentType&&(a=a.toLowerCase());a=f.createElement(a);a.setAttribute("rel","stylesheet");a.setAttribute("href",h.getAttribute("href"));c.appendChild(a)}};(function(){function c(){new g}"complete"===b.readyState||"interactive"===b.readyState?c():b.addEventListener&&b.addEventListener("DOMContentLoaded",c,!1)})();}).call(this);
