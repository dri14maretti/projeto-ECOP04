// import { AuthService } from './../../../services/auth.service';
// import { Component, OnInit } from '@angular/core';

@Component({
  selector: "app-ver-perfil",
  templateUrl: "./ver-perfil.component.html",
  styleUrls: ["./ver-perfil.component.scss"],
})
export class VerPerfilComponent implements OnInit {
  constructor(private authService: AuthService) {}

  ngOnInit(): void {
    this.authService.displayMessage("Teste realizado com sucesso!", false);
  }
}
