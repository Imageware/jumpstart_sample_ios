//
//  MainView.swift
//  gmi_ios_sample
//
//  Created by Vitalii Poponov on 11/3/20.
//

import SwiftUI

struct MainView: View {
    
    
    @ObservedObject var viewModel: MainViewModel
    
    init(viewModel: MainViewModel = MainViewModel()) {
        self.viewModel = viewModel
        
    }
    
    var body: some View {
        Form {

            Section(header: Text("GMI Server URL")) {
                TextField("GMI Server URL", text: self.$viewModel.serverURL)
                TextField("GMI UserManager URL", text: self.$viewModel.userManagerURL)
            }

            Section(header: Text("Client info")) {
                TextField("GMI Client ID", text: self.$viewModel.clientID)
                TextField("GMI Client secret", text: self.$viewModel.clientSecret)
            }

            Section(header: Text("")) {
                TextField("GMI Tenant Code", text: self.$viewModel.tenantCode)
                TextField("GMI Application Code", text: self.$viewModel.applicationCode)
            }
            
            Button(action: { }) { Text("Initialize GMI SDK") }
            
            Section(header: Text("Email / User ID")) {
                TextField("ExampleCode", text: self.$viewModel.email)
            }
            
            Button(action: { viewModel.checkEmail() }) { Text("Check Email") }
            
            Button(action: { }) { Text("Register User") }
            
//            Button(action: { }) { Text("Syncronize pending enrolls and alerts") }
//            
//            Button(action: { }) { Text("Perform next expected item") }
        }.alert(isPresented: self.$viewModel.showingAlert) {
            Alert(title: Text("Message"), message: Text(self.viewModel.alertTitle), dismissButton: .default(Text("Dismiss")))
        }
    }
}

