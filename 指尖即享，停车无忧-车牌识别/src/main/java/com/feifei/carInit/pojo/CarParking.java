package com.feifei.carInit.pojo;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;


@Data
@AllArgsConstructor
@NoArgsConstructor
public class CarParking {

        private String id;

        private String carPlate;

        private String startTimer;

        private String endTimer;

        private Boolean isParking;

        private Boolean isAccount;

}
