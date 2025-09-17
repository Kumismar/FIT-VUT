using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace project.DAL.Migrations
{
    /// <inheritdoc />
    public partial class ATUPList_OnDeleteUpdates : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_ATLists_Activities_ActivityId",
                table: "ATLists");

            migrationBuilder.DropForeignKey(
                name: "FK_ATLists_Tags_TagId",
                table: "ATLists");

            migrationBuilder.DropForeignKey(
                name: "FK_UPLists_Projects_ProjectId",
                table: "UPLists");

            migrationBuilder.DropForeignKey(
                name: "FK_UPLists_Users_UserId",
                table: "UPLists");

            migrationBuilder.AddForeignKey(
                name: "FK_ATLists_Activities_ActivityId",
                table: "ATLists",
                column: "ActivityId",
                principalTable: "Activities",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_ATLists_Tags_TagId",
                table: "ATLists",
                column: "TagId",
                principalTable: "Tags",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_UPLists_Projects_ProjectId",
                table: "UPLists",
                column: "ProjectId",
                principalTable: "Projects",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);

            migrationBuilder.AddForeignKey(
                name: "FK_UPLists_Users_UserId",
                table: "UPLists",
                column: "UserId",
                principalTable: "Users",
                principalColumn: "Id",
                onDelete: ReferentialAction.Cascade);
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_ATLists_Activities_ActivityId",
                table: "ATLists");

            migrationBuilder.DropForeignKey(
                name: "FK_ATLists_Tags_TagId",
                table: "ATLists");

            migrationBuilder.DropForeignKey(
                name: "FK_UPLists_Projects_ProjectId",
                table: "UPLists");

            migrationBuilder.DropForeignKey(
                name: "FK_UPLists_Users_UserId",
                table: "UPLists");

            migrationBuilder.AddForeignKey(
                name: "FK_ATLists_Activities_ActivityId",
                table: "ATLists",
                column: "ActivityId",
                principalTable: "Activities",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_ATLists_Tags_TagId",
                table: "ATLists",
                column: "TagId",
                principalTable: "Tags",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_UPLists_Projects_ProjectId",
                table: "UPLists",
                column: "ProjectId",
                principalTable: "Projects",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);

            migrationBuilder.AddForeignKey(
                name: "FK_UPLists_Users_UserId",
                table: "UPLists",
                column: "UserId",
                principalTable: "Users",
                principalColumn: "Id",
                onDelete: ReferentialAction.Restrict);
        }
    }
}
