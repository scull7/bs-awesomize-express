open Jest;

let expectPass = (title, testFn) =>
  testPromise(title, () =>
    testFn()
    |> Js.Promise.then_(result =>
         (
           switch (result) {
           | None => pass
           | Some(message) => fail(message)
           }
         )
         |> Js.Promise.resolve
       )
  );

describe("Awesomize Express", () => {
  describe("create", () => {
    let awesomize =
      Awesomize.make([|
        (
          "clientId",
          {
            read: Awesomize.Read.key("body"),
            sanitize: None,
            validate: [Awesomize.Validator.required, Awesomize.Validator.isInt],
            normalize: None,
          },
        ),
      |]);

    /*let decoder = json =>*/
      /*Json.Decode.{*/
        /*clientId: json |> field("clientId", int),*/
      /*};*/

    /*let encoder = Json.Encode.array(t =>*/
      /*Json.Encode.(*/
        /*object_([*/
          /*("id", t.id |> int),*/
          /*("created", t.created |> int),*/
          /*("updated", t.updated |> int),*/
          /*("clientId", t.clientId |> int),*/
          /*("foreignCredentialId", t.foreignCredentialId |> string),*/
          /*("roleId", t.roleId |> int),*/
          /*("roleCode", t.roleCode |> string),*/
          /*("roleDisplay", t.roleDisplay |> string),*/
          /*("scopeDd", t.scopeId |> int),*/
          /*("scopeCode", t.scopeCode |> string),*/
          /*("scopeDisplay", t.scopeDisplay |> string),*/
        /*])*/
      /*)*/
    /*);*/

    /*let handler = (req, {clientId, scopes}) =>*/
      /*Db.Express.get(req)*/
      /*|> Scope.createBatch(clientId, scopes)*/
      /*|> Util.Result.unsafeResolve;*/


    /*Awesomize.Express.create(*/
      /*~awesomize = awesomize,*/
      /*~decoder = decoder,*/
      /*~encode = encoder,*/
      /*~handler = handler*/
    /*)*/

    expectPass("", () => Js.Promise.resolve(None));





    /*let scopeArray = (*/
      /*"scopes",*/
      /*{*/
        /*read: Awesomize.Read.path(["body", "scopes"]),*/
        /*sanitize: None,*/
        /*validate: [Awesomize.Validator.required, ScopeArray.validator],*/
        /*normalize: None,*/
      /*},*/
    /*);*/

    /*let awesomize = Awesomize.make([|clientId, scopeArray|]);*/


    /*let unwrap = Awesomize.Express.create;*/

  });
});
