const formatTime = date => {
  const year = date.getFullYear()
  const month = date.getMonth() + 1
  const day = date.getDate()
  const hour = date.getHours()
  const minute = date.getMinutes()
  const second = date.getSeconds()

  return {
	  timer:[year, month, day].map(formatNumber).join('/') + ' ' + [hour, minute, second].map(formatNumber).join(':'),
	  jiluTimer:[hour, minute],
	  parkingTimer:[year,month,day,hour,minute]
  }
}

const formatNumber = n => {
  n = n.toString()
  return n[1] ? n : '0' + n
}

module.exports = {
  formatTime: formatTime
}
