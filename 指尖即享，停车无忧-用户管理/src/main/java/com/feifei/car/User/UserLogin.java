package com.feifei.car.User;


import com.baomidou.mybatisplus.annotation.IdType;
import com.baomidou.mybatisplus.annotation.TableField;
import com.baomidou.mybatisplus.annotation.TableId;
import com.baomidou.mybatisplus.annotation.TableName;
import lombok.Builder;
import lombok.Data;

@Data
@TableName("login")
public class UserLogin {

    @TableId(value = "id",type = IdType.AUTO)
    private int id;

    @TableField("user_name")
    private String userName;

    @TableField("pass_word")
    private String password;

    @TableField("email")
    private String email;

    public String getCarPlste() {
        return carPlste;
    }

    public void setCarPlste(String carPlste) {
        this.carPlste = carPlste;
    }

    @TableField("carplste")
    private String carPlste;

    public UserLogin(String carPlste) {
        this.carPlste = carPlste;
    }

    public UserLogin(String userName, String password, String email) {
        this.userName = userName;
        this.password = password;
        this.email = email;
    }

    public UserLogin(String userName, String password) {
        this.userName = userName;
        this.password = password;
    }

    public UserLogin(int id, String userName, String password, String email, String carPlste) {
        this.id = id;
        this.userName = userName;
        this.password = password;
        this.email = email;
        this.carPlste = carPlste;
    }

    public UserLogin(int id, String userName, String password, String email) {
        this.id = id;
        this.userName = userName;
        this.password = password;
        this.email = email;
    }
}
