<template>
	<view>
		<view class="main_continue">
			<view class="continue" v-for="(item,index) in carData" :key="index" @click="open(index)">
				<div class="msg_left">
					<div class="msg_left_top"></div>
					<div class="msg_left_bottom">
						<span class="data">设备ID:{{item.id}}</span>
						<span class="data">设备名:{{item.name}}</span>
					</div>
				</div>
				<div class="msg_right">
					<div class="msg_right_top">
						<span style="display: block;margin:20px 0 10px 0;color: red;">设备运行状态</span>
						<view v-if="yuyue[index]">
							<switch disabled="true" :checked="yuyue[index]" name="switch" />
							预约中
						</view>
						<view v-else>
							<switch disabled="true" :checked="item.status" name="switch" />
							<text v-if="item.status" style="color: green;margin-left: 10px;">运行中</text>
							<text v-else style="color: black;margin-left: 10px;">空闲中</text>
						</view>
					</div>
					<div class="msg_right_bottom">
						<div v-if="item.status">
							<text style="display: block;margin-top: 10px;">{{item.startNowTimer}}</text>
							<text style="display: block;margin-top: 10px;">已运行:{{dealItem(item)}}</text>
						</div>
						<div v-else class="msg_right_bottom">
							<span style="display: block;margin:10px 0 10px 0;">上次运行时间:</span>
							 {{dealItemd(item)}}
						</div>
					</div>

				</div>

			</view>
		</view>
		<uni-popup ref="popup" type="dialog">
			<view class="yuyue">
				<ul>
					<li><text> 预约车位 </text></li>
					<li>设备ID:{{clickCar.id}}</li>
					<li>设备名字:{{clickCar.name}}</li>
					<!-- 是否运行 -->
					<li v-if="clickCar.status">
						<switch  disabled="true" :checked="clickCar.status" name="switch" />
						<text style="color:green;">运行中</text>
					</li>
					<li v-else>
						<view v-if="!yuyue[clickCar.id]">
							<text style="color:red;">请选择进站时间:</text>
							<picker 
							    mode="date" 
							    :value="date" 
								fields="day"
							    :start="startDate" 
							    :end="endDate"     
							    @change="bindDateChange">
									<view>{{ Ytime }}</view>
							</picker>
						</view>
						<view v-else>
							<switch  disabled="true" :checked="yuyue[clickCar.id]" name="switch" />
							<text style="color:green;">预约中</text>
						</view>
					</li>
					<li>
						<div style="display: flex;">
							<button type="primary" @click="close" style="width: 40%;background-color:#fbffff;color:black;">取消</button>
							<button type="primary" @click="confirm(clickCar.id)" style="width: 40%;background-color:#409EFF;color: black;">确认</button>
						</div>
					</li>
				</ul>
			</view>
		</uni-popup>
	</view>
</template>

<script>
	import { formatTime } from './../utils/util.js' 
	import mqtt, { log } from './../utils/mqtt.js'
	const aliyunOpt = require("../utils/aliyun/aliyun_connect.js");
	
	export	default {
		data(){
			return{
				title:'停车管理系统',
				date:[
					{id:0,name:'车位一',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:1,name:'车位二',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:2,name:'车位三',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
					{id:3,name:'车位四',startNowTimer:"",startTimer:[0,0],endTimer:[0,0],status:false},
				],
				yuyue:[0,0,0,0],
				userCarData:[],
				carData:this.origin_carDate,
				dealTimers:[],
				nowTImer:"---",
				runtime:0,
				runtimed:0,
				carStatus:true,
				clickCar:{},
				Ytime:this.getDate({format: true}),
				indicatorDots: true,
				autoplay: true,
				interval: 2000,
				duration: 500,
				data:{
					client:null,//记录重连的次数
					reconnectCounts:0,//MQTT连接的配置
					options:{
						protocolVersion: 4, //MQTT连接协议版本
						clean: false,
						reconnectPeriod: 1000, //1000毫秒，两次重新连接之间的间隔
						connectTimeout: 30 * 1000, //1000毫秒，两次重新连接之间的间隔
						resubscribe: true, //如果连接断开并重新连接，则会再次自动订阅已订阅的主题（默认true）
						clientId: '',
						password: '',
						username: '',
					  },
					  aliyunInfo: {
						productKey: 'i4c2LeyEp9M', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						deviceName: 'weichat_booking', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						deviceSecret: '29877a488d421e7e7e21436a9321ab7e', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						pubTopic: '/i4c2LeyEp9M/weichat_booking/user/update', //发布消息的主题
						subTopic: '/i4c2LeyEp9M/weichat_booking/user/get', //订阅消息的主题
					  }
				},
				getCarDataUrl:"",// 获取数据的url
				getCarBookData:"http://127.0.0.1:8081/car/getData",
				loginStatus:0,// 登录状态
				user_name:"",
				pass_word:""
				
			}
		},
		props:["origin_carDate","pull"],
		methods:{
			dealTimer(carItem){
				let carStartTimer = carItem.startTimer;
				let carEndTimer = carItem.endTimer;
				return ((carEndTimer[0]-carEndTimer[0])*60 + carEndTimer[1]-carEndTimer[1]);
			},
			// 正在运行算出正在运行的时间
			dealItem(item){
				const that = this;
				const date = this.date;
				if((that.dealTimers[0] - item.startTimer[0])>0) return (that.dealTimers[1] - item.startTimer[1])
				return (that.dealTimers[0] - item.startTimer[0] - 1)*60 + (60 - item.startTimer[1]) + that.dealTimers[1]
			},
			// 如果停止运行了 算出运行后的时间
			dealItemd(item){
				const that = this;
				const date = this.date;
				if((that.dealTimers[0] - item.startTimer[0])>0) return (item.endTimer[1]- item.startTimer[1])
				return  (item.endTimer[0] - item.startTimer[0] - 1)*60 + (60 - item.startTimer[1]) + item.endTimer[1]
			},
			// 暂时没有用到
			updateTImer(){
				const that = this;
				const date = this.date;
				date.map((item,index)=>{
					const hous = 0;
					const mins = 0;
					console.log(that.runtime);
					// that.runtime = Math.abs(that.dealTimers[0] - item.startTimer[0])*60 + Math.abs(that.dealTimers[1]-item.startTimer[1]);
					// that.runtimed = Math.abs(item.endTimer[0] - item.startTimer[0])*60 +  Math.abs(item.endTimer[1] - item.startTimer[1]);
				})
			},
			open(index){
				this.clickCar = this.carData[index];
				this.$refs.popup.open();
				this.Ytime = this.getDate({format: true});
				console.log(index);
				console.log(this.Ytime)
			},
			close() {
				this.$refs.popup.close();
			},
			// 处理预约事件
			async axiosGetPost(url,method,data,index){
				await this.handleLogin();
				const that = this;
				let bookingData;
				let pushData;
				
				/**
				 * 检查用户是否登录
				 * 
				 * */
				const carData = that.date[index];
				if(!this.loginStatus){
					uni.showToast({
						title:"请先登录",
						icon:'error'
					})
					uni.redirectTo({
						url:"/pages/login/login"
					})
					return;
				}
				
				/** 
				* 处理用户的预约 
				**/
				url = `http://127.0.0.1:8081/car/booking?id=${this.clickCar.id}&phone=${this.user_name}`
				await uni.request({
					url:url,
					data:data,
					method:method,
					header: {
						"content-type": "application/x-www-form-urlencoded"
					},
					success(res) {
						if(res.data.code){
							bookingData = res.data.data;
							uni.showToast({
								title:"预约成功",
								icon:"success"
							})
							
							// 发送数据
							that.data.client.subscribe(that.data.aliyunInfo.subTopic,function(err){
								pushData = `carData:${carData.id}_1_phone_${that.user_name}`;
								that.data.client.publish(that.data.aliyunInfo.pubTopic, JSON.stringify(pushData))	
								console.log(pushData)
							})
							// 同步预约数据
							uni.$emit("breakBookingData");
							
							// 重新获取预约的数据
							that.getCarData( that.getCarBookData );
							
						}else{
							uni.showToast({
								title:res.data.msg,
								icon:'error'
							})
						}
						
					},fail(res) {
						uni.showToast({
							title:"获取用户数据失败",
							icon:"error"
						})
					}
				})
			},
			// 用户点击确认按钮
			confirm(index) {
				// 处理预约事件
                this.axiosGetPost("","GET",{},index);
				this.$refs.popup.close()	
			},
			// 获取时间用户选择的时间
			bindDateChange(e) {
				this.Ytime = e.detail.value.substr(0, 10);
				// console.log(this.Ytime)
				// this.time = this.Ytime;
			},
			// 获取当前的时间
			getDate(type) {
				const date = new Date();
				let year = date.getFullYear();
				let month = date.getMonth() + 1;
				let day = date.getDate();
				if (type === 'start') {
					year = year - 10;
				} else if (type === 'end') {
					year = year + 10;
				}
				month = month > 9 ? month : '0' + month;
				return `${year}-${month}-${day}`;
			},
			connectAliYun(){
					// clearTimeout(time);
					const that = this;
					const carData = this.date;	
					let clientOpt = aliyunOpt.getAliyunIotMqttClient({
					  productKey: that.data.aliyunInfo.productKey,
					  deviceName: that.data.aliyunInfo.deviceName,
					  deviceSecret: that.data.aliyunInfo.deviceSecret,
					  regionId: that.data.aliyunInfo.regionId,
					  port: that.data.aliyunInfo.port,
					});
					
					let host = 'wxs://' + clientOpt.host;
					this.data.options.clientId = clientOpt.clientId;
					this.data.options.password = clientOpt.password;
					this.data.options.username = clientOpt.username;
					this.data.client = mqtt.connect(host, this.data.options);		
					this.data.client.on('connect', function (connack) {
						uni.showToast({
							title: '连接成功'
						})
						console.log("连接成功");
					})
				},
				// 处理预约数据
				yuyueProcess( data ){
					const that = this;
					data.map((item,index)=>{
						that.yuyue[index] = item.booking;
					})
				},
				getCarData( requestUrl ){
					const that = this;
					uni.request({
						url:requestUrl,
						data:{},
						method:'GET',
						header: {
							"content-type": "application/x-www-form-urlencoded"
						},
						success(res) {
							that.userCarData = res.data.data;
							that.yuyueProcess( res.data.data);
							console.log(res.data)
						},fail(res) {
							uni.showToast({
								title:"获取用户数据失败",
								icon:"error"
							})
						}
					})
				},
				handleBookingEvent(dataId){
					const that = this;
					let pushData = "";
					console.log("预约成功id:"+dataId.id);
					that.data.client.subscribe(that.data.aliyunInfo.subTopic,function(err){
						pushData = `carData:${dataId.id}_0_phone_${dataId.phone}`;
						that.data.client.publish(that.data.aliyunInfo.pubTopic, JSON.stringify(pushData))
						console.log(pushData)
					})
				},
				handleLogin(){
					const that = this;
					return new Promise((resolve,reject)=>{
						uni.getStorage({
							key:"userInfo",
							success(val){
								uni.request({
									url:"http://127.0.0.1:8081/user/getUserInfo",
									data:val.data,
									method:'POST',
									header: {
										"content-type": "application/x-www-form-urlencoded"
									},
									success(res) {
										if(res.data.code){
											that.loginStatus = 1;
											
											that.user_name = res.data.data.userName;
											that.pass_word = res.data.data.password;
											console.log("登录成功")
										}else{
											that.loginStatus = 0;
										}
										resolve(1);
									},fail(res) {
										uni.showToast({
											title:"获取用户数据失败",
											icon:"error"
										})
										reject(0);
									}
								})
							},fail() {
								that.loginStatus = 0;
							}
						})
					})
				},
				handleYuyue(){
					const that = this;
					uni.navigateTo({
						url:"/pages/userbook/userbook",
						events:{
							cancelBooking:function(sendData){
								that.test()
							}
						}
					})
				}
			
		},
		// 这里主要用于监听上面的刷新时间 当刷新的时候就会触发这里
		watch:{
			pull:{
				handler(){
					const data = new Date();
					const date = this.date;
					const that = this;
					const newData = formatTime(data);
					this.nowTImer = newData.timer;
					this.dealTimers = newData.jiluTimer;
					console.log(this.dealTimers)
					this.carData = this.origin_carDate;
					this.getCarData( this.getCarBookData );
					this.handleLogin();
					// this.updateTImer();
				},
				deep:true,
			}
		},
		// 暂时没用用到 目前仅用于查询时间
		created(){
			const that = this;
			const getCarData = "http://127.0.0.1:8081/car/getData";
			
			let data = new Date();
			const newData = formatTime(data);
			
			this.handleLogin();
			
			uni.$on("handleBookingEvent",(data)=>{
				that.handleBookingEvent(data);
			});
			console.log("test")
			console.log(newData.timer);
			console.log(newData.jiluTimer);
			console.log("数据刷新")
			
			this.connectAliYun();
			
			this.getCarData( getCarData );
		},
		// none function printf time
		updated() {
			const data = new Date();
			const date = this.date;
			const that = this;
			const newData = formatTime(data);
			// console.log("test")
			// console.log(newData.timer);
			// console.log(newData.jiluTimer);
			// this.updateTImer();
		}
	}

</script>
	
<style>
	.main_continue{
		width: 100%;
		height: 150px;
		background-color: #fcfcfc;
		display: flex;
		justify-content: center;
		flex-wrap: wrap;
	}
	.continue{
		display: block;
		width: 90%;
		height: 100%;
		margin-top: 20px;
		background-color: #fff;
		border-radius: 20px;
		box-shadow: 1px 2px 3px 4px #fff4f7;
		display: flex;
		justify-content: center;
	}
	.data{
		display: block;
		text-align: center;
		margin-top: 4px;
		color: #000000;
	}
	.msg_left{
		width: 50%;
		height: 100%;
	}
	.msg_right{
		width: 50%;
		height: 100%;
	}
	.msg_left_top{
		width: 100%;
		height: 65%;
		background-size: 50% 90%;
		border-radius: 50%;
		background-image: url(../static/car.png);
		background-repeat: no-repeat;
		background-position: center;
	}
	.yuyue{
		width: 300px;
		height: 400px;
		margin: 0 auto;
		border-radius: 10px;
		background-color: #f5f5f5;
		-webkit-box-shadow: 1px 2px 3px 4px #ffffff;
	}
	ul{
		width: 100%;
		height: 100%;
	}
	ul li{
		text-align: center;
		list-style: none;
		width: 100%;
		height: 20%;
		text-align: cneter;
		/* background-color: #f0dee0; */
	}
	ul li:first-child{
		line-height: 50px;
		font-size: 20px;
		color:#d43c28;
	}
	
</style>
