<template>
	<view>
		<view class="main_continue">
			<text style="margin-top: 20px;">现在时间：{{nowTimer}}</text>
			<view class="continue" v-for="(item,index) in date" :key="index">
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
						<switch disabled="true" :checked="item.status" name="switch" />
						<text v-if="item.status" style="color: green;margin-left: 10px;">运行中</text>
						<text v-else style="color: black;margin-left: 10px;">空闲中</text>
					</div>
					<div class="msg_right_bottom">
						<div v-if="item.status">
							<text style="display: block;margin-top: 10px;">{{item.startNowTimer}}</text>
							<text style="display: block;margin-top: 10px;">已运行:&nbsp;{{runTime[index]}}&nbsp;分钟</text>
						</div>
						<div v-else class="msg_right_bottom">
							<span style="display: block;margin:10px 0 10px 0;">上次运行时间:</span>
							 {{dealTimer(item)}}&nbsp;&nbsp;分钟
						</div>
					</div>

				</div>

			</view>
		</view>
	</view>
</template>

<script>
	import mqtt from "./../../utils/mqtt.js"
	import { formatTime } from './../../utils/util.js'
	const aliyunOpt = require("../../utils/aliyun/aliyun_connect.js");
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
				carStatus:true,
				nowTimer:"2023/1/3 18:50:00",
				dealTimers:[0,0],
				runTime:[],
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
						deviceName: 'weichat', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						deviceSecret: '90d13b23ce47fd5f2dab1a5371247167', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						regionId: 'cn-shanghai', //阿里云连接的三元组 ，请自己替代为自己的产品信息!!
						pubTopic: '/i4c2LeyEp9M/weichat/user/update', //发布消息的主题
						subTopic: '/i4c2LeyEp9M/weichat/user/get', //订阅消息的主题
					  },
				},
			}
		},
		methods:{
			// 处理正在运行的时间
			dealTimer(carItem){
				let carStartTimer = carItem.startTimer;
				let carEndTimer = carItem.endTimer;
				if((carEndTimer[0] - carStartTimer[0])>0){
					return  (carEndTimer[1] - carStartTimer[1])
				}
				return (carEndTimer[0] - carStartTimer[0] - 1)*60 + (60 - carStartTimer[1]) + carEndTimer[1]
				// return ((carEndTimer[0]-carStartTimer[0])*60 + carEndTimer[1]-carStartTimer[1]);
			},
			// 处理已经运行的时间
			dealTimered(item){
				let carStartTimer = item.startTimer;
				let carEndTimer = this.dealTimers;
				if((carEndTimer[0] - carStartTimer[0])>0){
					return  (carEndTimer[1] - carStartTimer[1])
				}
				return (carEndTimer[0] - carStartTimer[0] - 1)*60 + (60 - carStartTimer[1]) + carEndTimer[1]
				// return ((carEndTimer[0]-carStartTimer[0])*60 + carEndTimer[1]-carStartTimer[1]); 
			},
			// 设置缓存的信息（将数据加入到缓存的数据中）
			cachesSetStorge(items){
				const length = items.length;
				items.map((item,index) =>{
					uni.setStorage({
						key:index + '',
						data:item,
						success(e) {
							console.log("已经储存信息");
						}
					})
				})
			},
			// 获取缓存的数据
			getCachesData(){
				const that = this;
				const carDa = this.date;
				carDa.map((item,i)=>{
					uni.getStorage({
						key:i + '',
						success(val) {
							that.date[i] = val.data;
						},
						fail() {
						uni.setStorage({
							key:i +'',
							data:carDa[1],
							success() {
								console.log("初始化数据成功");
							}
						})
					}
					})
				})
			},
			// 接入阿里云
			connectAliYun(){
				// clearTimeout(time);
				const that = this;
				const carData = this.date;
				this.getCachesData();
				const time = setInterval(()=>{
					const data = new Date();
					const runtime = that.runTime;
					const newData = formatTime(data);
					this.nowTimer = newData.timer;
					this.dealTimers = newData.jiluTimer;
					// console.log(this.nowTimer)
					const arr = carData.map((item,index) =>{
						if(item.status == true){
							runtime[index] = that.dealTimered(item);
						}
					})
				},1000)
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
						console.log(this.data.options)
				this.data.client.on('connect', function (connack) {
					wx.showToast({
						title: '连接成功'
					})
					console.log("连接成功");
				})
				//服务器连接异常的回调
				that.data.client.on("error", function (error) {
				  console.log(" 服务器 error 的回调" + error)
				})
				//服务器重连连接异常的回调
				that.data.client.on("reconnect", function () {
				  console.log(" 服务器 reconnect的回调")
				})
				//服务器连接异常的回调
				that.data.client.on("offline", function (errr) {
				  console.log(" 服务器offline的回调")
				})
			}
			
		},
		// 页面更换的时候加载
		onLoad(options) {
			this.connectAliYun();
		},
		onShow(){
			// 订阅格式 {"car":"1_1"}
			const that = this;
			this.data.client.on("message", (topic, payload)=> {
			  console.log(" 收到 topic:" + topic + " , payload :" + payload);
				try{
					const carData = this.date;
					console.log(carData)
					const data = JSON.parse(payload).car;
					const dealData = data.split("_");
					const mapData = carData.map((item,index) =>{
						if(dealData[0] == (item.id + '')){
							if(dealData[1] == "0" && carData[index].status == true) {
								carData[index].status = false;
								carData[index].endTimer = that.dealTimers;
								that.cachesSetStorge(carData);
							}
							else if(dealData[1] == "1" && carData[index].status == false){
								 carData[index].status = true;
								 carData[index].startTimer = that.dealTimers;
								 carData[index].startNowTimer = this.nowTimer;
								 that.cachesSetStorge(carData);
							}else{
								console.log("状态未发生变化");
								return 0;
							}
						}
						uni.$emit("unpdateDataBooking");
						return 1;
					})
				}catch(e){
					console.log('订阅消息的数据格式有误')
				}
			})
		},
		onPullDownRefresh(){
			// const arr = carData.map(item =>{
			// 	if(item.status == true){
			// 		that.runTime = that.dealTimered(item);
			// 	}
			// })
			this.getCachesData(4);
			setTimeout(function(){
				uni.stopPullDownRefresh();
			},2000)
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
		background-image: url(./../../static/car.png);
		background-repeat: no-repeat;
		background-position: center;
	}
	
</style>
