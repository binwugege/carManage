<template>
	<view>
		<view class="main_continue">
			<view class="continue" v-for="(item,index) in carData" :key="index">
				<span class="data">id:{{item.id}}</span>
				<span class="data">name:{{item.name}}</span>
				<div v-show="!item.status">
					{{item.startTimer}}
					{{item.endTimer}}
				</div>
			</view>
		</view>
	</view>
</template>

<script>
	import { formatTime } from './../utils/util.js' 
	
	export	default {
		data(){
			return{
				title:'停车管理系统',
				date:[
					{id:0,name:'设备一',startTimer:"",endTimer:"",status:true},
					{id:1,name:'设备二',startTimer:"",endTimer:"",status:false},
					{id:2,name:'设备三',startTimer:"",endTimer:"",status:false},
					{id:3,name:'设备四',startTimer:"",endTimer:"",status:false},
				],
				carStatus:true,
			}
		},
		props:["carData"],
		// 页面更换的时候加载
		onLoad(options) {
			console.log(options)
		},
		onShow(){
			let data = new Date();
			const newData = formatTime(data);
			console.log("test")
			console.log(newData.timer);
			console.log(newData.jiluTimer);
			console.log(this.julu);
			// this.jiluTimer = newData.jiluTimer
			console.log(this.julu);
		}
		
	}
</script>
	
<style>
	.main_continue{
		width: 100%;
		height: 450px;
		background-color: #fcfcfc;
		display: flex;
		flex-wrap: wrap;
		margin-top: 10px;
		justify-content: space-around;
		align-content: flex-start;
	}
	.continue{
		display: block;
		width: 43%;
		height: 43%;
		margin-top: 20px;
		background-color: #fff;
		border-radius: 10px;
		box-shadow: 1px 2px 3px 4px #fff4f7;
	}
	.data{
		display: block;
		text-align: center;
	}
</style>
