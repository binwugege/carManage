(global["webpackJsonp"]=global["webpackJsonp"]||[]).push([["component/desk"],{168:function(t,e,n){"use strict";n.r(e);var a=n(169),o=n(171);for(var r in o)["default"].indexOf(r)<0&&function(t){n.d(e,t,(function(){return o[t]}))}(r);n(173);var i,s=n(32),c=Object(s["default"])(o["default"],a["render"],a["staticRenderFns"],!1,null,null,null,!1,a["components"],i);c.options.__file="component/desk.vue",e["default"]=c.exports},169:function(t,e,n){"use strict";n.r(e);var a=n(170);n.d(e,"render",(function(){return a["render"]})),n.d(e,"staticRenderFns",(function(){return a["staticRenderFns"]})),n.d(e,"recyclableRender",(function(){return a["recyclableRender"]})),n.d(e,"components",(function(){return a["components"]}))},170:function(t,e,n){"use strict";var a;n.r(e),n.d(e,"render",(function(){return o})),n.d(e,"staticRenderFns",(function(){return i})),n.d(e,"recyclableRender",(function(){return r})),n.d(e,"components",(function(){return a}));try{a={uniPopup:function(){return n.e("uni_modules/uni-popup/components/uni-popup/uni-popup").then(n.bind(null,161))}}}catch(s){if(-1===s.message.indexOf("Cannot find module")||-1===s.message.indexOf(".vue"))throw s;console.error(s.message),console.error("1. 排查组件名称拼写是否正确"),console.error("2. 排查组件是否符合 easycom 规范，文档：https://uniapp.dcloud.net.cn/collocation/pages?id=easycom"),console.error("3. 若组件不符合 easycom 规范，需手动引入，并在 components 中注册该组件")}var o=function(){var t=this,e=t.$createElement,n=(t._self._c,t.__map(t.carData,(function(e,n){var a=t.__get_orig(e),o=e.status?t.dealItem(e):null,r=e.status?null:t.dealItemd(e);return{$orig:a,m0:o,m1:r}})));t.$mp.data=Object.assign({},{$root:{l0:n}})},r=!1,i=[];o._withStripped=!0},171:function(t,e,n){"use strict";n.r(e);var a=n(172),o=n.n(a);for(var r in a)["default"].indexOf(r)<0&&function(t){n.d(e,t,(function(){return a[t]}))}(r);e["default"]=o.a},172:function(t,e,n){"use strict";(function(t){var a=n(4),o=n(13);Object.defineProperty(e,"__esModule",{value:!0}),e.default=void 0;var r=a(n(115)),i=a(n(117)),s=n(98),c=d(n(87));function u(t){if("function"!==typeof WeakMap)return null;var e=new WeakMap,n=new WeakMap;return(u=function(t){return t?n:e})(t)}function d(t,e){if(!e&&t&&t.__esModule)return t;if(null===t||"object"!==o(t)&&"function"!==typeof t)return{default:t};var n=u(e);if(n&&n.has(t))return n.get(t);var a={},r=Object.defineProperty&&Object.getOwnPropertyDescriptor;for(var i in t)if("default"!==i&&Object.prototype.hasOwnProperty.call(t,i)){var s=r?Object.getOwnPropertyDescriptor(t,i):null;s&&(s.get||s.set)?Object.defineProperty(a,i,s):a[i]=t[i]}return a.default=t,n&&n.set(t,a),a}var l=n(88),f={data:function(){return{title:"停车管理系统",date:[{id:0,name:"车位一",startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:!1},{id:1,name:"车位二",startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:!1},{id:2,name:"车位三",startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:!1},{id:3,name:"车位四",startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:!1}],yuyue:[0,0,0,0],userCarData:[],carData:this.origin_carDate,dealTimers:[],nowTImer:"---",runtime:0,runtimed:0,carStatus:!0,clickCar:{},Ytime:this.getDate({format:!0}),indicatorDots:!0,autoplay:!0,interval:2e3,duration:500,data:{client:null,reconnectCounts:0,options:{protocolVersion:4,clean:!1,reconnectPeriod:1e3,connectTimeout:3e4,resubscribe:!0,clientId:"",password:"",username:""},aliyunInfo:{productKey:"i4c2LeyEp9M",deviceName:"weichat_booking",deviceSecret:"29877a488d421e7e7e21436a9321ab7e",regionId:"cn-shanghai",pubTopic:"/i4c2LeyEp9M/weichat_booking/user/update",subTopic:"/i4c2LeyEp9M/weichat_booking/user/get"}},getCarDataUrl:"",getCarBookData:"http://127.0.0.1:8081/car/getData",loginStatus:0,user_name:"",pass_word:""}},props:["origin_carDate","pull"],methods:{dealTimer:function(t){t.startTimer;var e=t.endTimer;return 60*(e[0]-e[0])+e[1]-e[1]},dealItem:function(t){var e=this;this.date;return e.dealTimers[0]-t.startTimer[0]>0?e.dealTimers[1]-t.startTimer[1]:60*(e.dealTimers[0]-t.startTimer[0]-1)+(60-t.startTimer[1])+e.dealTimers[1]},dealItemd:function(t){var e=this;this.date;return e.dealTimers[0]-t.startTimer[0]>0?t.endTimer[1]-t.startTimer[1]:60*(t.endTimer[0]-t.startTimer[0]-1)+(60-t.startTimer[1])+t.endTimer[1]},updateTImer:function(){var t=this,e=this.date;e.map((function(e,n){console.log(t.runtime)}))},open:function(t){this.clickCar=this.carData[t],this.$refs.popup.open(),this.Ytime=this.getDate({format:!0}),console.log(t),console.log(this.Ytime)},close:function(){this.$refs.popup.close()},axiosGetPost:function(e,n,a,o){var s=this;return(0,i.default)(r.default.mark((function i(){var c,u,d;return r.default.wrap((function(r){while(1)switch(r.prev=r.next){case 0:return r.next=2,s.handleLogin();case 2:if(c=s,d=c.date[o],s.loginStatus){r.next=8;break}return t.showToast({title:"请先登录",icon:"error"}),t.redirectTo({url:"/pages/login/login"}),r.abrupt("return");case 8:return e="http://127.0.0.1:8081/car/booking?id=".concat(s.clickCar.id,"&phone=").concat(s.user_name),r.next=11,t.request({url:e,data:a,method:n,header:{"content-type":"application/x-www-form-urlencoded"},success:function(e){e.data.code?(e.data.data,t.showToast({title:"预约成功",icon:"success"}),c.data.client.subscribe(c.data.aliyunInfo.subTopic,(function(t){u="carData:".concat(d.id,"_1_phone_").concat(c.user_name),c.data.client.publish(c.data.aliyunInfo.pubTopic,JSON.stringify(u)),console.log(u)})),t.$emit("breakBookingData"),c.getCarData(c.getCarBookData)):t.showToast({title:e.data.msg,icon:"error"})},fail:function(e){t.showToast({title:"获取用户数据失败",icon:"error"})}});case 11:case"end":return r.stop()}}),i)})))()},confirm:function(t){this.axiosGetPost("","GET",{},t),this.$refs.popup.close()},bindDateChange:function(t){this.Ytime=t.detail.value.substr(0,10)},getDate:function(t){var e=new Date,n=e.getFullYear(),a=e.getMonth()+1,o=e.getDate();return"start"===t?n-=10:"end"===t&&(n+=10),a=a>9?a:"0"+a,"".concat(n,"-").concat(a,"-").concat(o)},connectAliYun:function(){var e=this,n=(this.date,l.getAliyunIotMqttClient({productKey:e.data.aliyunInfo.productKey,deviceName:e.data.aliyunInfo.deviceName,deviceSecret:e.data.aliyunInfo.deviceSecret,regionId:e.data.aliyunInfo.regionId,port:e.data.aliyunInfo.port})),a="wxs://"+n.host;this.data.options.clientId=n.clientId,this.data.options.password=n.password,this.data.options.username=n.username,this.data.client=c.default.connect(a,this.data.options),this.data.client.on("connect",(function(e){t.showToast({title:"连接成功"}),console.log("连接成功")}))},yuyueProcess:function(t){var e=this;t.map((function(t,n){e.yuyue[n]=t.booking}))},getCarData:function(e){var n=this;t.request({url:e,data:{},method:"GET",header:{"content-type":"application/x-www-form-urlencoded"},success:function(t){n.userCarData=t.data.data,n.yuyueProcess(t.data.data),console.log(t.data)},fail:function(e){t.showToast({title:"获取用户数据失败",icon:"error"})}})},handleBookingEvent:function(t){var e=this,n="";console.log("预约成功id:"+t.id),e.data.client.subscribe(e.data.aliyunInfo.subTopic,(function(a){n="carData:".concat(t.id,"_0_phone_").concat(t.phone),e.data.client.publish(e.data.aliyunInfo.pubTopic,JSON.stringify(n)),console.log(n)}))},handleLogin:function(){var e=this;return new Promise((function(n,a){t.getStorage({key:"userInfo",success:function(o){t.request({url:"http://127.0.0.1:8081/user/getUserInfo",data:o.data,method:"POST",header:{"content-type":"application/x-www-form-urlencoded"},success:function(t){t.data.code?(e.loginStatus=1,e.user_name=t.data.data.userName,e.pass_word=t.data.data.password,console.log("登录成功")):e.loginStatus=0,n(1)},fail:function(e){t.showToast({title:"获取用户数据失败",icon:"error"}),a(0)}})},fail:function(){e.loginStatus=0}})}))},handleYuyue:function(){var e=this;t.navigateTo({url:"/pages/userbook/userbook",events:{cancelBooking:function(t){e.test()}}})}},watch:{pull:{handler:function(){var t=new Date,e=(this.date,(0,s.formatTime)(t));this.nowTImer=e.timer,this.dealTimers=e.jiluTimer,console.log(this.dealTimers),this.carData=this.origin_carDate,this.getCarData(this.getCarBookData),this.handleLogin()},deep:!0}},created:function(){var e=this,n="http://127.0.0.1:8081/car/getData",a=new Date,o=(0,s.formatTime)(a);this.handleLogin(),t.$on("handleBookingEvent",(function(t){e.handleBookingEvent(t)})),console.log("test"),console.log(o.timer),console.log(o.jiluTimer),console.log("数据刷新"),this.connectAliYun(),this.getCarData(n)},updated:function(){var t=new Date;this.date,(0,s.formatTime)(t)}};e.default=f}).call(this,n(2)["default"])},173:function(t,e,n){"use strict";n.r(e);var a=n(174),o=n.n(a);for(var r in a)["default"].indexOf(r)<0&&function(t){n.d(e,t,(function(){return a[t]}))}(r);e["default"]=o.a},174:function(t,e,n){}}]);
//# sourceMappingURL=../../.sourcemap/mp-weixin/component/desk.js.map
;(global["webpackJsonp"] = global["webpackJsonp"] || []).push([
    'component/desk-create-component',
    {
        'component/desk-create-component':(function(module, exports, __webpack_require__){
            __webpack_require__('2')['createComponent'](__webpack_require__(168))
        })
    },
    [['component/desk-create-component']]
]);
