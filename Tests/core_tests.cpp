#include <gtest/gtest.h>
#include <PositronEngineCore/Application.hpp>
#include <PositronEngineCore/Primitives/CubePrimitive.hpp>
#include <PositronEngineCore/Primitives/SpherePrimitive.hpp>
#include <PositronEngineCore/Primitives/PlatePrimitive.hpp>
#include <PositronEngineCore/Primitives/Model.hpp>

TEST(CubePrimitiveTest, Create)
{
    std::string name = "Cube";
    PositronEngine::CubePrimitive cube(name);
    EXPECT_EQ(cube.getName(), name);
    EXPECT_EQ(cube.getObjectType(), PositronEngine::ObjectType::primitive);
}

TEST(CubePrimitiveTest, GetAndSetMaterial)
{
    PositronEngine::CubePrimitive cube("Cube");
    std::shared_ptr<PositronEngine::Material> material = std::make_shared<PositronEngine::Material>();
    cube.setMaterial(material);
    EXPECT_EQ(cube.getMaterial(), material);
}

TEST(CubePrimitiveTest, GetMesh)
{
    PositronEngine::CubePrimitive cube("Cube");
    std::shared_ptr<PositronEngine::Mesh> mesh = cube.getMesh();
    EXPECT_NE(mesh, nullptr);
}

TEST(CubePrimitiveTest, CopyConstructorAndAssignmentOperator)
{
    PositronEngine::CubePrimitive cube1("Cube1");
    PositronEngine::CubePrimitive cube2 = cube1;
    EXPECT_EQ(cube1.getName(), cube2.getName());

    PositronEngine::CubePrimitive cube3("Cube3");
    cube3 = cube1;
    EXPECT_EQ(cube1.getName(), cube3.getName());
}


TEST(SpherePrimitiveTest, Create)
{
    std::string name = "Sphere";
    PositronEngine::SpherePrimitive sphere(name);
    EXPECT_EQ(sphere.getName(), name);
    EXPECT_EQ(sphere.getObjectType(), PositronEngine::ObjectType::primitive);

}

TEST(SpherePrimitiveTest, GetAndSetMaterial)
{
    PositronEngine::SpherePrimitive sphere("Sphere");
    std::shared_ptr<PositronEngine::Material> material = std::make_shared<PositronEngine::Material>();
    sphere.setMaterial(material);
    EXPECT_EQ(sphere.getMaterial(), material);
}



TEST(SpherePrimitiveTest, GetMesh)
{
    PositronEngine::SpherePrimitive sphere("Sphere");
    std::shared_ptr<PositronEngine::Mesh> mesh = sphere.getMesh();
    EXPECT_NE(mesh, nullptr);
}

TEST(SpherePrimitiveTest, CopyConstructorAndAssignmentOperator)
{
    PositronEngine::SpherePrimitive sphere1("Sphere1");
    PositronEngine::SpherePrimitive sphere2 = sphere1;
    EXPECT_EQ(sphere1.getName(), sphere2.getName());

    PositronEngine::SpherePrimitive sphere3("Sphere3");
    sphere3 = sphere1;
    EXPECT_EQ(sphere1.getName(), sphere3.getName());
}



TEST(PlatePrimitiveTest, Create)
{
    std::string name = "Sphere";
    PositronEngine::SpherePrimitive sphere(name);
    EXPECT_EQ(sphere.getName(), name);
    EXPECT_EQ(sphere.getObjectType(), PositronEngine::ObjectType::primitive);
}

TEST(PlatePrimitiveTest, GetAndSetMaterial)
{
    PositronEngine::PlatePrimitive plate("Plate");
    std::shared_ptr<PositronEngine::Material> material = std::make_shared<PositronEngine::Material>();
    plate.setMaterial(material);
    EXPECT_EQ(plate.getMaterial(), material);
}

TEST(PlatePrimitiveTest, GetMesh)
{
    PositronEngine::PlatePrimitive plate("Plate");;
    std::shared_ptr<PositronEngine::Mesh> mesh = plate.getMesh();
    EXPECT_NE(mesh, nullptr);
}

TEST(PlatePrimitiveTest, CopyConstructorAndAssignmentOperator)
{
    PositronEngine::PlatePrimitive plate1("Plate1");;
    PositronEngine::PlatePrimitive plate2 = plate1;
    EXPECT_EQ(plate1.getName(), plate2.getName());

    PositronEngine::PlatePrimitive plate3("Plate3");
    plate3 = plate1;
    EXPECT_EQ(plate1.getName(), plate3.getName());
}


TEST(ModelPrimitiveTest, Create)
{
    std::string name = "Model";
    PositronEngine::Model model("wrongPath", name);
    EXPECT_EQ(model.getName(), name);
    EXPECT_EQ(model.checkSuccessUpload(), false);
}

TEST(ModelPrimitiveTest, GetMaterial)
{
    std::string name = "Model";
    PositronEngine::Model model("path", name);
    EXPECT_EQ(model.getName(), name);
    EXPECT_EQ(model.checkSuccessUpload(), true);

    EXPECT_NE(model.getMaterial(), nullptr);
}

TEST(TextureTest, CreateDiffuseTexture)
{
    PositronEngine::Texture2D diffuse_texture("path", PositronEngine::TextureType::diffuse);
    EXPECT_EQ(diffuse_texture.getType(), PositronEngine::TextureType::diffuse);
}

TEST(TextureTest, CreateNormalTexture)
{
    PositronEngine::Texture2D normal_texture("path", PositronEngine::TextureType::normal);
    EXPECT_EQ(normal_texture.getType(), PositronEngine::TextureType::normal);
}

TEST(TextureTest, CreateRoughnessTexture)
{
    PositronEngine::Texture2D roughness_texture("path", PositronEngine::TextureType::roughness);
    EXPECT_EQ(roughness_texture.getType(), PositronEngine::TextureType::roughness);
}

TEST(TextureTest, CreateDisplasementTexture)
{
    PositronEngine::Texture2D displasement_texture("path", PositronEngine::TextureType::displasement);
    EXPECT_EQ(displasement_texture.getType(), PositronEngine::TextureType::displasement);
}

TEST(TextureTest, CreateShadowTexture)
{
    PositronEngine::Texture2D shadow_texture("path", PositronEngine::TextureType::shadow);
    EXPECT_EQ(shadow_texture.getType(), PositronEngine::TextureType::shadow);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
