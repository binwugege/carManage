<template>
	<view>
		<div class="continer">
			<span v-for="(item,index) in carArry" :key="index" :class="{'a':item.status}"></span>
		</div>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				carArry:[],
			}
		},
		onLoad(carData) {
			const cardata = JSON.parse(carData.data);
			this.carArry = cardata;
		},
		methods: {
			
		}
	}
</script>

<style>
	.continer{
		width: 300px;
		height: 300px;
		background-color: red;
		margin: 100px auto;
		display: flex;
		flex-wrap: wrap;
		list-style: none;
		justify-content: space-between;
		align-items: center;
	}
	span{
		width: 45%;
		height: 45%;
		display: block;
		background-color:green;
	}
	.a{
		background-color:pink;
	}
</style>
