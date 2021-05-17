using GnollHackServer.Data;
using Microsoft.AspNetCore.Identity;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

// For more information on enabling Web API for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace GnollHackServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class LoginController : ControllerBase
    {
        private SignInManager<IdentityUser> _signInManager;

        public LoginController(SignInManager<IdentityUser> signInManager)
        {
            _signInManager = signInManager;
        }

        // POST api/<LoginController>
        [HttpPost]
        public async Task<ActionResult> Post([FromBody] LoginCredentials loginCredentials)
        {
            var result = await _signInManager.PasswordSignInAsync(loginCredentials.UserName, loginCredentials.Password, true, lockoutOnFailure: false);
            if(result == Microsoft.AspNetCore.Identity.SignInResult.Success)
            {
                return Ok();
            }
            else
            {
                return Forbid();
            }
        }
    }
}
