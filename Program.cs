using FireWarningWebApp.Data;
using FireWarningWebApp.Models;
using Microsoft.EntityFrameworkCore;

var builder = WebApplication.CreateBuilder(args);


builder.Services.AddDbContext<AppDbContext>(options =>
    options.UseSqlite("Data Source=firewarning.db"));

builder.Services.AddScoped<UserService>();
builder.Services.AddControllers();

var app = builder.Build();


app.UseDefaultFiles();
app.UseStaticFiles();
app.MapControllers();
app.MapPost("/register", async (UserDto user, UserService userService) =>
{
    var result = await userService.RegisterUserAsync(user.Username, user.Password);
    if (result)
        return Results.Json(new { message = "Registratie succesvol!" });
    else
        return Results.BadRequest(new { message = "Gebruiker bestaat al." });
});







Console.WriteLine("API beschikbaar op /register");

app.Run(); 

public record UserDto(string Username, string Password);



