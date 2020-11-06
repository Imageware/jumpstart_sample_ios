//
//  MainViewModel.swift
//  gmi_ios_sample
//
//  Created by Vitalii Poponov on 11/4/20.
//
import SwiftUI
import Foundation
import GMI

class MainViewModel: ObservableObject {
    
    private var deviceServiceManager: DeviceServiceManagerProtocol = DeviceServiceManager()

    @Published var showingAlert = false
    var alertTitle: String = ""
    
    @Published var serverURL: String = ""
    @Published var userManagerURL: String = ""
    
    @Published var clientID: String = ""
    @Published var clientSecret: String = ""
    
    @Published var tenantCode: String = ""
    @Published var applicationCode: String = ""
    
    @Published var email: String = ""
    
    var config: Config {
        return Config(presetName: "", presetTitle: "", serverUrl: self.serverURL, clientSecret: self.clientSecret, clientID: self.clientID, userManagerUrl: self.userManagerURL, defaultTenantCode: self.tenantCode, applicationCode: self.applicationCode)
    }
    var account: Account?
    
    func checkEmail() {
        let account = Account()
        account.config = config
        account.email = email
        deviceServiceManager.personForEmail(account: account) { _ in
//            switch response {
//            case .success(let person):
//                break
//            case .error(let error):
//                break
//            @unknown default: break
//            }
            print("popo")
//            switch response {
//            case .success(let person):
//                account.id = person.id
//                account.tenantEnrollServer = person.tenantEnrollServer
//                account.enrollServer = person.enrollServer
//                account.tenantVerifyServer = person.tenantVerifyServer
//                account.verifyServer = person.verifyServer
//                self.account = account
//            case .error:
//                self.alertTitle = "Email not found in the selected Region"
//                self.showingAlert.toggle()
//            @unknown default: break
//            }
        }
    }
    
}
